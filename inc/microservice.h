#ifndef MICROSERVICE_H
#define MICROSERVICE_H

#include <openhd/mavlink.h>
#include "constants.h"

#include <array>

#include <boost/asio.hpp>

class Microservice {
public:
    Microservice(boost::asio::io_service &io_service);
    
    virtual ~Microservice() {}

    void connect();
    void reconnect(const boost::system::error_code& error);
    void start_receive();    
    void send_heartbeat(const boost::system::error_code& error);
    void send_system_time(const boost::system::error_code& error);
    void set_sysid(uint8_t sysid);
    void set_compid(uint8_t compid);

    virtual void setup();
    void handle_receive(const boost::system::error_code& error, std::size_t recvlen);
    void handle_write(const boost::system::error_code &error);
    virtual void process_mavlink_message(mavlink_message_t msg) = 0;

protected:
    uint8_t m_sysid;
    uint8_t m_compid;
    char m_recv_buf[1024];

    bool m_connected = false;

    boost::asio::io_service &m_io_service;

    std::shared_ptr<boost::asio::ip::tcp::socket> m_socket;

    mavlink_status_t m_mavlink_status;

private:
    std::chrono::steady_clock::time_point m_boot_time;


    std::chrono::seconds m_heartbeat_interval;
    boost::asio::steady_timer m_heartbeat_timer;

    std::chrono::seconds m_sys_time_interval;
    boost::asio::steady_timer m_sys_time_timer;

    std::chrono::seconds m_reconnect_interval;
    boost::asio::steady_timer m_reconnect_timer;
};

#endif
