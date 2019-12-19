#ifndef CAMERAMICROSERVICE_H
#define CAMERAMICROSERVICE_H

#include <openhd/mavlink.h>
#include "constants.h"

#include <array>

#include <boost/asio.hpp>

#include "microservice.h"

class CameraMicroservice: public Microservice {
public:
    CameraMicroservice(boost::asio::io_service &io_service);

    virtual void setup();

    virtual void process_mavlink_message(mavlink_message_t msg);
};

#endif
