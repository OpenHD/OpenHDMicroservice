#ifndef POWER_H
#define POWER_H

#include <openhd/mavlink.h>
#include "constants.h"

#include <array>

#include <boost/asio.hpp>

#include "microservice.h"

class PowerMicroservice: public Microservice {
public:
    PowerMicroservice(boost::asio::io_service &io_service);

    virtual void setup();

    void send_openhd_ground_power(const boost::system::error_code& error);

    virtual void process_mavlink_message(mavlink_message_t msg);
};

#endif
