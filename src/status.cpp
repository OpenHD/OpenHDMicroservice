#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/reboot.h>

#include <iostream>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include <openhd/mavlink.h>

#include "status.h"

#include "constants.h"

#include "localmessage_t.h"


constexpr uint8_t SERVICE_COMPID = MAV_COMP_ID_USER3;

StatusMicroservice::StatusMicroservice(boost::asio::io_service &io_service): Microservice(io_service), m_udp_socket(io_service) {
    set_compid(SERVICE_COMPID);

    m_udp_socket.open(boost::asio::ip::udp::v4());
    m_udp_socket.bind(boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 50000));
}


void StatusMicroservice::setup() {
    std::cout << "StatusMicroservice::setup()" << std::endl;
    Microservice::setup();
    start_udp_read();
}


void StatusMicroservice::start_udp_read() {
    std::cerr << "StatusMicroservice::start_udp_read()" << std::endl;

    m_udp_socket.async_receive(boost::asio::buffer(data, max_length),
                               boost::bind(&StatusMicroservice::handle_udp_read,
                                           this,
                                           boost::asio::placeholders::error,
                                           boost::asio::placeholders::bytes_transferred));
}


void StatusMicroservice::handle_udp_read(const boost::system::error_code& error,
                                         size_t bytes_transferred) {
    std::cerr << "StatusMicroservice::handle_udp_read()" << std::endl;

    if (!error) {
        if (bytes_transferred == sizeof(localmessage_t)) {
            localmessage_t local_message;

            memcpy(&local_message, data, bytes_transferred);

            std::string message = (const char*)local_message.message;
            MAV_SEVERITY severity = (MAV_SEVERITY)local_message.level;

            send_status_message(severity, message);
        }
    }
    start_udp_read();
}



/*
 * Used to send a status message with a log level to any connected devices, and also store the message locally.
 * 
 * This makes it possible for the system to "replay" all messages when a new device connects to the system and
 * needs to see messages that were generated before that point.
 * 
 * This is primarily done so that early boot messages are preserved and available for review at any time, but is
 * useful at all times to ensure the user knows what's going on.
 */
void StatusMicroservice::send_status_message(MAV_SEVERITY severity, std::string message) {
    std::cout << "StatusMicroservice::send_status_message" << std::endl;

    auto timestamp = boost::posix_time::microsec_clock::local_time().time_of_day().total_milliseconds();

    /*
     * Store the message first, so we can send all local messages to any GCS that requests them later on
     */
    StatusMessage m;
    m.message = message;
    m.severity = severity;
    m.timestamp = timestamp;
    m_status_messages.push_back(m);

    /*
     * Now send it out
     */
    uint8_t raw[MAVLINK_MAX_PACKET_LEN];
    int len = 0;

    char text[50] = {0};
    strncpy(text, message.c_str(), sizeof(text));
    if (text[49] != '\0') {
        text[49] = '\0';
    }


    mavlink_message_t outgoing_msg;
    mavlink_msg_openhd_status_message_pack(this->m_sysid, this->m_compid, &outgoing_msg, 0, 0, severity, text, timestamp);    
    len = mavlink_msg_to_send_buffer(raw, &outgoing_msg);

    this->m_socket->async_send(boost::asio::buffer(raw, len),
                               boost::bind(&Microservice::handle_write,
                                           this,
                                           boost::asio::placeholders::error));
}


void StatusMicroservice::process_mavlink_message(mavlink_message_t msg) {
        switch (msg.msgid) {
        case MAVLINK_MSG_ID_OPENHD_STATUS_MESSAGE: {

            mavlink_openhd_status_message_t status;
            mavlink_msg_openhd_status_message_decode(&msg, &status);

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
                case OPENHD_CMD_GET_STATUS_MESSAGES: {
                    std::cout << "OPENHD_CMD_GET_STATUS_MESSAGES" << std::endl;
                    uint8_t raw[MAVLINK_MAX_PACKET_LEN];
                    int len = 0;

                    // acknowledge the command, then reply
                    mavlink_message_t ack;
                    mavlink_msg_command_ack_pack(this->m_sysid, // mark the message as being from the local system ID
                                                 this->m_compid,  // and from this component
                                                 &ack,
                                                 OPENHD_CMD_GET_STATUS_MESSAGES, // the command we're ack'ing
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
                    

                    for (auto & message : m_status_messages) {
                        uint8_t raw[MAVLINK_MAX_PACKET_LEN];
                        int len = 0;

                        char text[50] = {0};
                        strncpy(text, message.message.c_str(), sizeof(text));
                        if (text[49] != '\0') {
                            text[49] = '\0';
                        }


                        mavlink_message_t outgoing_msg;
                        mavlink_msg_openhd_status_message_pack(this->m_sysid, this->m_compid, &outgoing_msg, msg.sysid, msg.compid, message.severity, text, message.timestamp);    
                        len = mavlink_msg_to_send_buffer(raw, &outgoing_msg);

                        this->m_socket->async_send(boost::asio::buffer(raw, len),
                                                boost::bind(&Microservice::handle_write,
                                                            this,
                                                            boost::asio::placeholders::error));
                    }
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

