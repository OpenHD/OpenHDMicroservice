#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>

#include <boost/regex.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

#include <openhd/mavlink.h>

#include "sensor.h"

#include "constants.h"


constexpr uint8_t SERVICE_COMPID = MAV_COMP_ID_USER4;



SensorMicroservice::SensorMicroservice(boost::asio::io_service &io_service): 
    Microservice(io_service),
    m_temperature_interval(10000), 
    m_temperature_timer(io_service, m_temperature_interval) {
    set_compid(SERVICE_COMPID);
}


void SensorMicroservice::setup() {
    std::cout << "SensorMicroservice::setup()" << std::endl;
    Microservice::setup();

    detect_ds18b20();


    if (!m_temperature_sensors.empty()) {
        std::cout << "SensorMicroservice: detected temperature sensors, enabling data stream" << std::endl;
        this->m_temperature_timer.async_wait(boost::bind(&SensorMicroservice::send_openhd_temperature, this));
    }

}




void SensorMicroservice::send_openhd_temperature() {
    std::cout << "SensorMicroservice::send_openhd_temperature" << std::endl;

    uint8_t raw[MAVLINK_MAX_PACKET_LEN];
    int len = 0;

    /*
     * Retrieve temperature from detected sensors
     */
    int8_t temperature[8] = { -127, -127, -127, -127, -127, -127, -127, -127 };

    int index = 0;

    for (auto& sensor : m_temperature_sensors) {
        auto sensor_type = sensor.first;
        auto identifier = sensor.second;

        switch (sensor_type) {
            case TemperatureSensorDS18B20: {
                std::stringstream device_file("/sys/bus/w1/devices/");
                device_file << identifier;
                device_file << "w1_slave";

                std::ifstream t(device_file.str());
                std::string raw_value((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
                
                boost::smatch result;

                boost::regex r{ "t=([\\d]+)"};
                if (!boost::regex_match(raw_value, result, r)) {
                    break;
                }

                if (result.size() != 2) {
                    break;
                }

                std::string _t = result[1];
                temperature[index] = atoi(_t.c_str());
                
                break;
            }
            default: {
                break;
            }
        }

        index++;
    }

    mavlink_message_t outgoing_msg;
    mavlink_msg_openhd_temperature_sensor_pack(this->m_sysid,
                                               this->m_compid,
                                               &outgoing_msg,
                                               0,
                                               0,
                                               temperature);

    len = mavlink_msg_to_send_buffer(raw, &outgoing_msg);

    this->m_socket->async_send(boost::asio::buffer(raw, len),
                               boost::bind(&Microservice::handle_write,
                                           this,
                                           boost::asio::placeholders::error));

    this->m_temperature_timer.expires_at(this->m_temperature_timer.expires_at() + this->m_temperature_interval);
    this->m_temperature_timer.async_wait(boost::bind(&SensorMicroservice::send_openhd_temperature, this));
}



void SensorMicroservice::process_mavlink_message(mavlink_message_t msg) {
        switch (msg.msgid) {
        case MAVLINK_MSG_ID_PARAM_REQUEST_LIST: {
            // will be used for service-specific settings
            std::cout << "MAVLINK_MSG_ID_PARAM_REQUEST_LIST" << std::endl;

            mavlink_param_request_list_t request;
            mavlink_msg_param_request_list_decode(&msg, &request);
            break;
        }
        default: {
            break;
        }
    }
}


void SensorMicroservice::detect_ds18b20() {
    std::stringstream device_file("/sys/bus/w1/devices/");
    
    for(auto & p : boost::filesystem::directory_iterator(device_file.str())) {
        auto dir = p.path().string();
        if (dir.rfind("28-", 0) == 0) {
            m_temperature_sensors.push_back(std::make_pair(TemperatureSensorDS18B20, dir));
        }
    }
}
