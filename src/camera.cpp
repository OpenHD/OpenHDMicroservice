

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#include <iostream>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include <openhd/mavlink.h>

#include "camera.h"
#include "constants.h"

#define SERVICE_COMPID MAV_COMP_ID_CAMERA


CameraMicroservice::CameraMicroservice(boost::asio::io_service &io_service): Microservice(io_service) {
    set_compid(SERVICE_COMPID);
}


void CameraMicroservice::setup() {}


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
                    
                    boost::system::error_code err;
                    this->m_socket.send(boost::asio::buffer(raw, len), 0, err);

                    // ... then reply
                    mavlink_message_t outgoing_msg;
                    mavlink_msg_openhd_camera_settings_pack(this->m_sysid, // mark the message as being from the local system ID
                                                            this->m_compid,  // and from this component
                                                            &outgoing_msg,
                                                            brightness,
                                                            contrast,
                                                            saturation,
                                                            msg.sysid,
                                                            msg.compid);
                    len = mavlink_msg_to_send_buffer(raw, &outgoing_msg);

                    this->m_socket.send(boost::asio::buffer(raw, len), 0, err);
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
                    
                    boost::system::error_code err;
                    this->m_socket.send(boost::asio::buffer(raw, len), 0, err);

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

