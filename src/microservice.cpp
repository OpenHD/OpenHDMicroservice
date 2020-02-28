#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#include <iostream>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
using namespace boost::asio;


#include <openhd/mavlink.h>


#include "microservice.h"

#include "constants.h"



Microservice::Microservice(boost::asio::io_service &io_service): m_io_service(io_service),
                                                                 m_socket(new boost::asio::ip::tcp::socket(io_service)), 
                                                                 m_boot_time(boost::posix_time::microsec_clock::local_time().time_of_day().total_milliseconds()),
                                                                 m_heartbeat_interval(5), 
                                                                 m_heartbeat_timer(io_service, m_heartbeat_interval), 
                                                                 m_sys_time_interval(1), 
                                                                 m_sys_time_timer(io_service, m_sys_time_interval),
                                                                 m_reconnect_interval(1), 
                                                                 m_reconnect_timer(io_service, m_reconnect_interval) {}

void Microservice::set_sysid(uint8_t sysid) {
    this->m_sysid = sysid;
}

void Microservice::set_compid(uint8_t compid) {
    this->m_compid = compid;
}

void Microservice::reconnect(const boost::system::error_code& error) {
    connect();
    
    m_reconnect_timer.expires_at(m_reconnect_timer.expires_at() + m_reconnect_interval);
    m_reconnect_timer.async_wait(boost::bind(&Microservice::reconnect, 
                                       this, 
                                       boost::asio::placeholders::error));
}

void Microservice::connect() {
    if (m_connected) {
        return;
    }

    m_socket.reset(new boost::asio::ip::tcp::socket(m_io_service));

    boost::system::error_code error;

    m_socket->open(ip::tcp::v4());
    m_socket->connect(ip::tcp::endpoint(boost::asio::ip::address::from_string(ROUTER_ADDRESS), ROUTER_PORT), error);

    if (error) {
        std::cerr << "Connection failed: " << error.message() << std::endl;
        m_socket->close();
    } else {
        m_connected = true;
        start_receive();
    }
}

void Microservice::setup() {
    std::cout << "Microservice::setup()" << std::endl;
    this->m_heartbeat_timer.async_wait(boost::bind(&Microservice::send_heartbeat, 
                                       this, 
                                       boost::asio::placeholders::error));

    this->m_reconnect_timer.async_wait(boost::bind(&Microservice::reconnect, 
                                       this, 
                                       boost::asio::placeholders::error));
}

void Microservice::start_receive() {
    this->m_socket->async_receive(boost::asio::buffer(this->m_recv_buf, sizeof(this->m_recv_buf)), 
                                 boost::bind(&Microservice::handle_receive, 
                                             this,
                                             boost::asio::placeholders::error,
                                             boost::asio::placeholders::bytes_transferred));
}

void Microservice::send_heartbeat(const boost::system::error_code& error) {
    std::cout << "Microservice::send_heartbeat" << std::endl;
    uint8_t raw[MAVLINK_MAX_PACKET_LEN];
    int len = 0;

    mavlink_message_t outgoing_msg;
    mavlink_msg_heartbeat_pack(this->m_sysid, this->m_compid, &outgoing_msg, MAV_TYPE_GENERIC, MAV_AUTOPILOT_INVALID, 0, 0, 0);
    len = mavlink_msg_to_send_buffer(raw, &outgoing_msg);


    this->m_socket->async_send(boost::asio::buffer(raw, len),
                                boost::bind(&Microservice::handle_write,
                                            this,
                                            boost::asio::placeholders::error));

    m_heartbeat_timer.expires_at(m_heartbeat_timer.expires_at() + m_heartbeat_interval);
    this->m_heartbeat_timer.async_wait(boost::bind(&Microservice::send_heartbeat, 
                                                   this, 
                                                   boost::asio::placeholders::error));
}

void Microservice::send_system_time(const boost::system::error_code& error) {
    std::cout << "Microservice::send_system_time" << std::endl;
    uint8_t raw[MAVLINK_MAX_PACKET_LEN];
    int len = 0;

    auto now = boost::posix_time::microsec_clock::local_time().time_of_day().total_milliseconds();

    mavlink_message_t outgoing_msg;
    mavlink_msg_system_time_pack(this->m_sysid, this->m_compid, &outgoing_msg, now, m_boot_time);
    len = mavlink_msg_to_send_buffer(raw, &outgoing_msg);


    this->m_socket->async_send(boost::asio::buffer(raw, len),
                                boost::bind(&Microservice::handle_write,
                                            this,
                                            boost::asio::placeholders::error));

    m_sys_time_timer.expires_at(m_sys_time_timer.expires_at() + m_sys_time_interval);
    this->m_sys_time_timer.async_wait(boost::bind(&Microservice::send_system_time, 
                                                   this, 
                                                   boost::asio::placeholders::error));
}

void Microservice::handle_write(const boost::system::error_code &error) {
    if (error) {
        m_connected = false;
    } 
}

void Microservice::handle_receive(const boost::system::error_code& error, std::size_t recvlen) {
    if (error) {
        m_connected = false;
        return;
    } 
    
    mavlink_message_t msg;
    for (int i = 0; i < recvlen; i++) {
        uint8_t res = mavlink_parse_char(MAVLINK_COMM_0, (uint8_t)m_recv_buf[i], &msg, &m_mavlink_status);
        if (res) {
            process_mavlink_message(msg);
        }
    }

    start_receive();
}
