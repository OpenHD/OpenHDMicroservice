#ifndef CAMERAMICROSERVICE_H
#define CAMERAMICROSERVICE_H

#include <openhd/mavlink.h>

#include <array>
#include <stdexcept>
#include <vector>

#include <boost/asio.hpp>

#include "openhd-camera.hpp"
#include "openhd-microservice.hpp"


#include "camerastream.h"


class CameraMicroservice: public Microservice {
public:
    CameraMicroservice(boost::asio::io_service &io_service, PlatformType platform);

    void setup();
    void process_manifest();

    void process_mavlink_message(mavlink_message_t msg);

private:
    PlatformType m_platform_type;

    std::vector<CameraStream> m_camera_streams;
};

#endif
