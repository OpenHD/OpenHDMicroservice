#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <openhd/mavlink.h>

#define ROUTER_ADDRESS "127.0.0.1"
#define ROUTER_PORT 5761


typedef enum PlatformType {
    PlatformTypeRaspberryPi,
    PlatformTypeJetson,
    PlatformTypeNanoPi,
    PlatformTypeRockchip,
    PlatformTypePC,
    PlatformTypeUnknown
} PlatformType;


typedef enum CameraType {
    CameraTypeRaspberryPiCSI,
    CameraTypeJetsonCSI,
    CameraTypeRockchipCSI,
    CameraTypeUVC,
    CameraTypeIP,
    CameraTypeUnknown
} CameraType;


struct CameraEndpoint {
    std::string device_node;
    bool support_h264 = false;
    bool support_h265 = false;
    bool support_mjpeg = false;
    bool support_raw = false;

    std::vector<std::string> formats;
};

struct Camera {
    CameraType type;
    std::string name = "unknown";
    std::string vendor = "unknown";
    std::string vid;
    std::string pid;
    // for USB this is the bus number, for CSI it's the connector number
    std::string bus;
};


#endif
