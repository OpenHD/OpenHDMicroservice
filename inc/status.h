#ifndef STATUS_H
#define STATUS_H

#include <openhd/mavlink.h>
#include "constants.h"

#include <array>
#include <vector>

#include <boost/asio.hpp>

#include "microservice.h"

struct StatusMessage {
    std::string message;
    int sysid;
    int compid;
    MAV_SEVERITY severity;
    uint64_t timestamp;
};


class StatusMicroservice: public Microservice {
public:
    StatusMicroservice(boost::asio::io_service &io_service);

    void setup();

    void start_udp_read();

    void handle_udp_read(const boost::system::error_code& error,
                         size_t bytes_transferred);

    void send_status_message(MAV_SEVERITY severity, std::string message);

    virtual void process_mavlink_message(mavlink_message_t msg);
    
 private:

    enum { max_length = 1024 };
    char data[max_length];

    std::vector<StatusMessage> m_status_messages;

    boost::asio::ip::udp::socket m_udp_socket;

    std::string m_openhd_version = "unknown";
};

#endif
