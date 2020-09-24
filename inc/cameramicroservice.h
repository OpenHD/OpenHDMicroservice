#ifndef CAMERAMICROSERVICE_H
#define CAMERAMICROSERVICE_H

#include <openhd/mavlink.h>
#include "constants.h"

#include <array>
#include <stdexcept>
#include <vector>

#include <boost/asio.hpp>

#include "camerastream.h"
#include "microservice.h"

class CameraMicroservice: public Microservice {
public:
    CameraMicroservice(boost::asio::io_service &io_service, PlatformType platform);

    void setup();
    void process_manifest();

    void process_mavlink_message(mavlink_message_t msg);

private:
    PlatformType m_platform_type;

    CameraType camera_type_from_string(std::string str);

    std::vector<CameraStream> m_camera_streams;
};

#endif
