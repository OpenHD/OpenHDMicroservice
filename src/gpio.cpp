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

extern "C" {
    #include <bcm2835.h>
}

#define SERVICE_COMPID MAV_COMP_ID_USER2

#define GPIO5 5
#define GPIO6 6
#define GPIO12 12
#define GPIO13 13
#define GPIO16 16
#define GPIO19 19
#define GPIO26 26
#define GPIO32 32


GPIOMicroservice::GPIOMicroservice(boost::asio::io_service &io_service): Microservice(io_service), m_status_interval(5), m_status_timer(io_service, m_status_interval) {
    set_compid(SERVICE_COMPID);
    m_status_interval = boost::posix_time::seconds(5);
}


void GPIOMicroservice::setup() {
    std::cout << "GPIOMicroservice::setup()" << std::endl;
    Microservice::setup();

    if (!bcm2835_init()) {
        throw std::runtime_error("bcm2835 gpio init failed");
    }
    bcm2835_gpio_fsel(GPIO5, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(GPIO6, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(GPIO12, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(GPIO13, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(GPIO16, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(GPIO19, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(GPIO26, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(GPIO32, BCM2835_GPIO_FSEL_OUTP);

    this->m_status_timer.async_wait(boost::bind(&GPIOMicroservice::send_openhd_gpio_state, 
                                                this, 
                                                boost::asio::placeholders::error));
}


void GPIOMicroservice::send_openhd_gpio_state(const boost::system::error_code& error) {
    std::cout << "GPIOMicroservice::send_openhd_gpio_state" << std::endl;
    uint8_t raw[MAVLINK_MAX_PACKET_LEN];
    int len = 0;

    /* 
     * Pin states are a single uint8_t bit pattern since we only support 8 specific pins for simplicity
     * and safety. Allowing apps to modify arbitrary pins could cause loss of vehicle control, for
     * example the UART pin(s) are carring RC control data.
     */

    uint8_t pins = 0;

    pins |= bcm2835_gpio_lev(GPIO5)  & 1 << 0;
    pins |= bcm2835_gpio_lev(GPIO6)  & 1 << 1;
    pins |= bcm2835_gpio_lev(GPIO12) & 1 << 2;
    pins |= bcm2835_gpio_lev(GPIO13) & 1 << 3;
    pins |= bcm2835_gpio_lev(GPIO16) & 1 << 4;
    pins |= bcm2835_gpio_lev(GPIO19) & 1 << 5;
    pins |= bcm2835_gpio_lev(GPIO26) & 1 << 6;
    pins |= bcm2835_gpio_lev(GPIO32) & 1 << 7;

    mavlink_message_t outgoing_msg;
    mavlink_msg_openhd_gpio_state_pack(this->m_sysid,
                                       this->m_compid, 
                                       &outgoing_msg,
                                       0, 
                                       0,
                                       pins);
    len = mavlink_msg_to_send_buffer(raw, &outgoing_msg);

    boost::system::error_code err;
    this->m_socket.send(boost::asio::buffer(raw, len), 0, err);

    this->m_status_timer.expires_at(this->m_status_timer.expires_at() + this->m_status_interval);
    this->m_status_timer.async_wait(boost::bind(&GPIOMicroservice::send_openhd_gpio_state, 
                                                this, 
                                                boost::asio::placeholders::error));
}


void GPIOMicroservice::process_mavlink_message(mavlink_message_t msg) {
        switch (msg.msgid) {
        case MAVLINK_MSG_ID_PARAM_REQUEST_LIST: {
            // will be used for service-specific settings
            std::cout << "MAVLINK_MSG_ID_PARAM_REQUEST_LIST" << std::endl;

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
                case OPENHD_CMD_SET_GPIOS: {
                    std::cout << "OPENHD_CMD_SET_GPIOS" << std::endl;

                    auto pin = (uint8_t)command.param1;
                
                    auto gpio5  = pin & 1 << 0;
                    auto gpio6  = pin & 1 << 1;
                    auto gpio12 = pin & 1 << 2;
                    auto gpio13 = pin & 1 << 3;
                    auto gpio16 = pin & 1 << 4;
                    auto gpio19 = pin & 1 << 5;
                    auto gpio26 = pin & 1 << 6;
                    auto gpio32 = pin & 1 << 7;


                    bcm2835_gpio_write(GPIO5,  gpio5);
                    bcm2835_gpio_write(GPIO6,  gpio6);
                    bcm2835_gpio_write(GPIO12, gpio12);
                    bcm2835_gpio_write(GPIO13, gpio13);
                    bcm2835_gpio_write(GPIO16, gpio16);
                    bcm2835_gpio_write(GPIO19, gpio19);
                    bcm2835_gpio_write(GPIO26, gpio26);
                    bcm2835_gpio_write(GPIO32, gpio32);


                    uint8_t raw[MAVLINK_MAX_PACKET_LEN];
                    int len = 0;

                    // acknowledge the command, no reply
                    mavlink_message_t ack;
                    mavlink_msg_command_ack_pack(this->m_sysid, // mark the message as being from the local system ID
                                                 this->m_compid,  // and from this component
                                                 &ack,
                                                 OPENHD_CMD_SET_GPIOS, // the command we're ack'ing
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
