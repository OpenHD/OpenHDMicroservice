#ifndef SENSOR_MICROSERVICE_H
#define SENSOR_MICROSERVICE_H

#include <openhd/mavlink.h>
#include "constants.h"

#include <array>

#include <boost/asio.hpp>

#include "microservice.h"

#include "RTIMULib.h"
#include "RTIMUMagCal.h"

typedef enum TemperatureSensor {
    TemperatureSensorNone,
    TemperatureSensorDS18B20
} TemperatureSensor;



class SensorMicroservice: public Microservice {
public:
    SensorMicroservice(boost::asio::io_service &io_service, PlatformType platform);

    void setup();

    virtual void process_mavlink_message(mavlink_message_t msg);

    void detect_ds18b20();

    
    void send_openhd_temperature();
private:
    std::vector<std::pair<TemperatureSensor, std::string > > m_temperature_sensors;

    std::chrono::milliseconds m_temperature_interval;
    boost::asio::steady_timer m_temperature_timer;

};

#endif
