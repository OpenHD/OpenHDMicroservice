#ifndef GPIO_MICROSERVICE_H
#define GPIO_MICROSERVICE_H

#include <openhd/mavlink.h>

#include <array>

#include <boost/asio.hpp>

#include "openhd-microservice.hpp"


class GPIOMicroservice: public Microservice {
public:
    GPIOMicroservice(boost::asio::io_service &io_service, PlatformType platform);

    void setup();

    virtual void process_mavlink_message(mavlink_message_t msg);

    void send_openhd_gpio_state();
};

#endif
