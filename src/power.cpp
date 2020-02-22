#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

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


#define SERVICE_COMPID MAV_COMP_ID_USER1


PowerMicroservice::PowerMicroservice(boost::asio::io_service &io_service): Microservice(io_service), m_status_interval(5), m_status_timer(io_service, m_status_interval) {
    set_compid(SERVICE_COMPID);
    m_status_interval = boost::posix_time::seconds(5);
}


void PowerMicroservice::setup() {
    std::cout << "PowerMicroservice::setup()" << std::endl;
    Microservice::setup();
    this->m_status_timer.async_wait(boost::bind(&PowerMicroservice::send_openhd_ground_power, 
                                                this, 
                                                boost::asio::placeholders::error));
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

    mavlink_message_t outgoing_msg;
    mavlink_msg_openhd_ground_power_pack(this->m_sysid, this->m_compid, &outgoing_msg, (float)vin / 1000.0, (float)vout / 1000.0, (float)vbat / 1000.0, (float)iout / 1000.0, bat_type);
    len = mavlink_msg_to_send_buffer(raw, &outgoing_msg);

    boost::system::error_code err;
    this->m_socket.send(boost::asio::buffer(raw, len), 0, err);

    this->m_status_timer.expires_at(this->m_status_timer.expires_at() + this->m_status_interval);
    this->m_status_timer.async_wait(boost::bind(&PowerMicroservice::send_openhd_ground_power, 
                                                this, 
                                                boost::asio::placeholders::error));
}


void PowerMicroservice::process_mavlink_message(mavlink_message_t msg) {}
