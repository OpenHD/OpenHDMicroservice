#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#include <iostream>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include <openhd/mavlink.h>

#include "gpio.h"

#include "constants.h"


#define SERVICE_COMPID MAV_COMP_ID_USER2


GPIOMicroservice::GPIOMicroservice(boost::asio::io_service &io_service): Microservice(io_service) {}


void GPIOMicroservice::setup() {}

void GPIOMicroservice::process_mavlink_message(mavlink_message_t msg) {}
