#ifndef POWER_H
#define POWER_H

#include <openhd/mavlink.h>

#include <array>

#include <boost/asio.hpp>

#include "openhd-microservice.hpp"

typedef enum PowerSensor {
    PowerSensorNone,
    PowerSensorLifepo4weredPi,
    PowerSensorINA2XX
} PowerSensor;

class PowerMicroservice: public Microservice {
public:
    PowerMicroservice(boost::asio::io_service &io_service, PlatformType platform);

    void setup();

    void send_openhd_power(const boost::system::error_code& error);

    virtual void process_mavlink_message(mavlink_message_t msg);
    
 private:
    std::chrono::seconds m_status_interval;
    boost::asio::steady_timer m_status_timer;
    PowerSensor m_sensor_type = PowerSensorNone;
};

#endif
