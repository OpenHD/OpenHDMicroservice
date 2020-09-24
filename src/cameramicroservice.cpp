

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#include <iostream>
#include <stdexcept>
#include <vector>
#include <fstream>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include <json.hpp>

#include <openhd/mavlink.h>

#include "cameramicroservice.h"
#include "camerastream.h"
#include "gstreamerstream.h"

#include "constants.h"


constexpr uint8_t SERVICE_COMPID = MAV_COMP_ID_CAMERA;


CameraMicroservice::CameraMicroservice(boost::asio::io_service &io_service, PlatformType platform): Microservice(io_service, platform) {
    set_compid(SERVICE_COMPID);
}


void CameraMicroservice::setup() {
    process_manifest();
}


void CameraMicroservice::process_manifest() {
    try {
        std::ifstream f("/tmp/camera_manifest");
        nlohmann::json j;
        f >> j;

        int port = 5600;

        for (auto _camera : j) {

            Camera camera;
            std::string camera_type = _camera["type"];
            camera.type = camera_type_from_string(camera_type);
            camera.name = _camera["name"];
            camera.vendor = _camera["vendor"];
            camera.vid = _camera["vid"];
            camera.pid = _camera["pid"];
            camera.bus = _camera["bus"];


            std::vector<CameraEndpoint> camera_endpoints;

            auto _endpoints = _camera["endpoints"];
            for (auto _endpoint : _endpoints) {
                CameraEndpoint endpoint;
                endpoint.device_node   = _endpoint["device_node"];
                endpoint.support_h264  = _endpoint["support_h264"];
                endpoint.support_h265  = _endpoint["support_h265"];
                endpoint.support_mjpeg = _endpoint["support_mjpeg"];
                endpoint.support_raw   = _endpoint["support_raw"];
                for (auto& format : _endpoint["formats"]) {
                    endpoint.formats.push_back(format);
                }

                camera_endpoints.push_back(endpoint);
            }

            switch (camera.type) {
                case CameraTypeRaspberryPiCSI:
                case CameraTypeJetsonCSI: 
                case CameraTypeIP:         
                case CameraTypeRockchipCSI:
                case CameraTypeUVC: {
                    GStreamerStream stream(m_io_service, m_platform_type, camera, camera_endpoints, port);
                    m_camera_streams.push_back(stream);
                    break;
                }
                default: {
                    std::cerr << "Unknown camera type, skipping" << std::endl;
                }
            }
            std::cerr << "Done processing camera" << std::endl;

            port++;
        }
    } catch (std::exception &ex) {
        // don't do anything, but send an error message to the user through the status service
        std::cerr << "Camera error: " << ex.what() << std::endl;
    }

}


void CameraMicroservice::process_mavlink_message(mavlink_message_t msg) {
    switch (msg.msgid) {
        case MAVLINK_MSG_ID_PARAM_REQUEST_LIST: {
            mavlink_param_request_list_t request;
            mavlink_msg_param_request_list_decode(&msg, &request);
            break;
        }
        case MAVLINK_MSG_ID_COMMAND_LONG: {
            mavlink_command_long_t command;
            mavlink_msg_command_long_decode(&msg, &command);

            // only process commands sent to this system or broadcast to all systems
            if ((command.target_system != this->m_sysid && command.target_system != 0)) {
                return;
            }

            // only process commands sent to this component or boadcast to all components on this system
            if ((command.target_component != this->m_compid && command.target_component != MAV_COMP_ID_ALL)) {
                return;
            }

            switch (command.command) {
                case OPENHD_CMD_GET_CAMERA_SETTINGS: {
                    uint8_t brightness = 128; //(uint8_t)settings.value("brightness", 128).toUInt();
                    uint8_t contrast   = 128; //(uint8_t)settings.value("contrast", 128).toUInt();
                    uint8_t saturation = 128; //(uint8_t)settings.value("saturation", 128).toUInt();


                    uint8_t raw[MAVLINK_MAX_PACKET_LEN];
                    int len = 0;

                    // acknowledge the command first...
                    mavlink_message_t ack;
                    mavlink_msg_command_ack_pack(this->m_sysid, // mark the message as being from the local system ID
                                                 this->m_compid, // and from this component
                                                 &ack,
                                                 OPENHD_CMD_GET_CAMERA_SETTINGS, // the command we're ack'ing
                                                 MAV_CMD_ACK_OK,
                                                 0,
                                                 0,
                                                 msg.sysid,   // address the ack to the senders system ID...
                                                 msg.compid); // ... and the senders component ID
                    len = mavlink_msg_to_send_buffer(raw, &ack);

                    this->m_socket->async_send(boost::asio::buffer(raw, len),
                                               boost::bind(&Microservice::handle_write,
                                                           this,
                                                           boost::asio::placeholders::error));

                    // ... then reply
                    mavlink_message_t outgoing_msg;
                    mavlink_msg_openhd_camera_settings_pack(this->m_sysid, // mark the message as being from the local system ID
                                                            this->m_compid,  // and from this component
                                                            &outgoing_msg,
                                                            msg.sysid,
                                                            msg.compid,
                                                            brightness,
                                                            contrast,
                                                            saturation);
                    len = mavlink_msg_to_send_buffer(raw, &outgoing_msg);

                    this->m_socket->async_send(boost::asio::buffer(raw, len),
                                               boost::bind(&Microservice::handle_write,
                                                           this,
                                                           boost::asio::placeholders::error));

                    break;
                }
                case OPENHD_CMD_SET_CAMERA_SETTINGS: {
                    /* this is how you would store a setting for the component when
                     * replying to a settings request
                     */
                    //QSettings settings;
                    //settings.beginGroup("OpenHDCamera");

                    //settings.setValue("brightness", (uint8_t)command.param1);
                    //settings.setValue("contrast", (uint8_t)command.param2);
                    //settings.setValue("saturation", (uint8_t)command.param3);

                    uint8_t raw[MAVLINK_MAX_PACKET_LEN];
                    int len = 0;

                    // acknowledge the command, no reply
                    mavlink_message_t ack;
                    mavlink_msg_command_ack_pack(this->m_sysid, // mark the message as being from the local system ID
                                                 this->m_compid,  // and from this component
                                                 &ack,
                                                 OPENHD_CMD_GET_CAMERA_SETTINGS, // the command we're ack'ing
                                                 MAV_CMD_ACK_OK,
                                                 0,
                                                 0,
                                                 msg.sysid, // send ack to the senders system ID...
                                                 msg.compid); // ... and the senders component ID
                    len = mavlink_msg_to_send_buffer(raw, &ack);

                    this->m_socket->async_send(boost::asio::buffer(raw, len),
                                               boost::bind(&Microservice::handle_write,
                                                           this,
                                                           boost::asio::placeholders::error));

                    break;
                }
                default: {
                    break;
                }
            }
            break;
        }
        default: {
            break;
        }
    }
}


CameraType CameraMicroservice::camera_type_from_string(std::string str) {
    if (str == "pi-csi") {
        return CameraTypeRaspberryPiCSI;
    } else if (str == "jetson-csi") {
        return CameraTypeJetsonCSI;
    } else if (str == "rockchip-csi") {
        return CameraTypeRockchipCSI;
    } else if (str == "uvc") {
        return CameraTypeUVC;
    } else if (str == "ip") {
        return CameraTypeIP;
    }

    return CameraTypeUnknown;
}


