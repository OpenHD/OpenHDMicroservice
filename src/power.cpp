#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/reboot.h>

#include <iostream>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include <openhd/mavlink.h>

#include "power.h"

#include "constants.h"


extern "C" {
    #include <lifepo4wered-data.h>
}


extern "C" {
    #include "ina2xx.h"
}


constexpr uint8_t SERVICE_COMPID = MAV_COMP_ID_USER1;

PowerMicroservice::PowerMicroservice(boost::asio::io_service &io_service): Microservice(io_service), m_status_interval(5), m_status_timer(io_service, m_status_interval) {
    set_compid(SERVICE_COMPID);
}


void PowerMicroservice::setup() {
    std::cout << "PowerMicroservice::setup()" << std::endl;
    Microservice::setup();

    if (read_lifepo4wered(VIN) >= 0) {
        m_sensor_type = PowerSensorLifepo4weredPi;
    } else if (ina2xx_init() >= 0) {
        m_sensor_type = PowerSensorINA2XX;
    }


    switch (m_sensor_type) {
        case PowerSensorNone: {
            break;
        }
        default: {
            this->m_status_timer.async_wait(boost::bind(&PowerMicroservice::send_openhd_ground_power, 
                                            this, 
                                            boost::asio::placeholders::error));
            break;
        }
    }
}


void PowerMicroservice::send_openhd_ground_power(const boost::system::error_code& error) {
    std::cout << "PowerMicroservice::send_openhd_ground_power" << std::endl;

    uint8_t raw[MAVLINK_MAX_PACKET_LEN];
    int len = 0;

    uint8_t bat_type = MAV_BATTERY_TYPE_LIFE;

    auto vin = read_lifepo4wered(VIN);
    auto vout = read_lifepo4wered(VOUT);
    auto iout = read_lifepo4wered(IOUT);
    auto vbat = read_lifepo4wered(VBAT);

    // don't send any message if an error is encountered
    if (out < 0 || iout < 0) {
        this->m_status_timer.expires_at(this->m_status_timer.expires_at() + this->m_status_interval);
        this->m_status_timer.async_wait(boost::bind(&PowerMicroservice::send_openhd_ground_power, 
                                                    this, 
                                                    boost::asio::placeholders::error));
    }

    mavlink_message_t outgoing_msg;
    mavlink_msg_openhd_ground_power_pack(this->m_sysid, this->m_compid, &outgoing_msg, 0, 0, (float)vin / 1000.0, (float)vout / 1000.0, (float)vbat / 1000.0, (float)iout / 1000.0, bat_type);
    len = mavlink_msg_to_send_buffer(raw, &outgoing_msg);

    this->m_socket->async_send(boost::asio::buffer(raw, len),
                               boost::bind(&Microservice::handle_write,
                                           this,
                                           boost::asio::placeholders::error));

    this->m_status_timer.expires_at(this->m_status_timer.expires_at() + this->m_status_interval);
    this->m_status_timer.async_wait(boost::bind(&PowerMicroservice::send_openhd_ground_power, 
                                                this, 
                                                boost::asio::placeholders::error));
}


void PowerMicroservice::process_mavlink_message(mavlink_message_t msg) {
        switch (msg.msgid) {
        case MAVLINK_MSG_ID_PARAM_REQUEST_LIST: {
            // will be used for service-specific settings
            std::cout << "MAVLINK_MSG_ID_PARAM_REQUEST_LIST" << std::endl;

            mavlink_param_request_list_t request;
            mavlink_msg_param_request_list_decode(&msg, &request);
            break;
        }
        case MAVLINK_MSG_ID_COMMAND_LONG: {
            mavlink_command_long_t command;
            mavlink_msg_command_long_decode(&msg, &command);

            // only process commands sent to this system or broadcast to all systems
            if ((command.target_system != this->m_sysid && command.target_system != 0)) {
                return;
            }

            // only process commands sent to this component or boadcast to all components on this system
            if ((command.target_component != this->m_compid && command.target_component != MAV_COMP_ID_ALL)) {
                return;
            }

            switch (command.command) {
                case OPENHD_CMD_POWER_REBOOT: {
                    std::cout << "OPENHD_CMD_POWER_REBOOT" << std::endl;
                    uint8_t raw[MAVLINK_MAX_PACKET_LEN];
                    int len = 0;

                    // acknowledge the command, no reply
                    mavlink_message_t ack;
                    mavlink_msg_command_ack_pack(this->m_sysid, // mark the message as being from the local system ID
                                                 this->m_compid,  // and from this component
                                                 &ack,
                                                 OPENHD_CMD_POWER_REBOOT, // the command we're ack'ing
                                                 MAV_CMD_ACK_OK,
                                                 0,
                                                 0,
                                                 msg.sysid, // send ack to the senders system ID...
                                                 msg.compid); // ... and the senders component ID
                    len = mavlink_msg_to_send_buffer(raw, &ack);
                    
                    this->m_socket->async_send(boost::asio::buffer(raw, len),
                                               boost::bind(&Microservice::handle_write,
                                                           this,
                                                           boost::asio::placeholders::error));

                    sync();
                    sleep(3);
                    reboot(RB_AUTOBOOT);
                    break;
                }
                case OPENHD_CMD_POWER_SHUTDOWN: {
                    std::cout << "OPENHD_CMD_POWER_SHUTDOWN" << std::endl;
                    uint8_t raw[MAVLINK_MAX_PACKET_LEN];
                    int len = 0;

                    // acknowledge the command, no reply
                    mavlink_message_t ack;
                    mavlink_msg_command_ack_pack(this->m_sysid, // mark the message as being from the local system ID
                                                 this->m_compid,  // and from this component
                                                 &ack,
                                                 OPENHD_CMD_POWER_SHUTDOWN, // the command we're ack'ing
                                                 MAV_CMD_ACK_OK,
                                                 0,
                                                 0,
                                                 msg.sysid, // send ack to the senders system ID...
                                                 msg.compid); // ... and the senders component ID
                    len = mavlink_msg_to_send_buffer(raw, &ack);
                    
                    this->m_socket->async_send(boost::asio::buffer(raw, len),
                                               boost::bind(&Microservice::handle_write,
                                                           this,
                                                           boost::asio::placeholders::error));

                    sync();
                    sleep(3);
                    reboot(RB_POWER_OFF);
                    break;
                }
                default: {
                    break;
                }
            }
            break;
        }
        default: {
            break;
        }
    }
}

