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
    void start_receive();    
    void send_heartbeat(const boost::system::error_code& error);
    void set_sysid(int8_t sysid);

    void setup();
    void handle_receive(const boost::system::error_code& error, std::size_t recvlen);
    virtual void process_mavlink_message(mavlink_message_t msg) = 0;

protected:
    int8_t m_sysid;

    char m_recv_buf[1024];

    boost::asio::ip::tcp::socket m_socket;

    mavlink_status_t m_mavlink_status;

private:
    boost::posix_time::seconds m_heartbeat_interval;
    boost::asio::deadline_timer m_heartbeat_timer;

};

#endif
