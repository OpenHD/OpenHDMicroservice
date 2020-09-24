
#include <iostream>
#include <iterator>
#include <boost/program_options.hpp>
#include <exception>
#include <fstream>

#include "boost/asio.hpp"
using namespace boost::asio;

#include "constants.h"
#include "microservice.h"
#include "power.h"
#include "camera.h"
#include "gpio.h"
#include "status.h"
#include "sensor.h"

#include "json.hpp"


// todo: move this and similar code in openhd-system to a central library pulled in with a git submodule.
PlatformType platform_type_from_string(std::string str) {
    if (str == "raspberrypi") {
        return PlatformTypeRaspberryPi;
    } else if (str == "jetson") {
        return PlatformTypeJetson;
    } else if (str == "nanopi") {
        return PlatformTypeNanoPi;
    } else if (str == "pc") {
        return PlatformTypePC;
    }

    return PlatformTypeUnknown;
}



int main(int argc, char *argv[]) {


    boost::asio::io_service io_service;

    Microservice * service = nullptr;

    try {
        boost::program_options::options_description desc("Options");

        desc.add_options()("help", "produce help message")
                          ("camera", "enable camera microservice")
                          ("power", "enable power sensor microservice")
                          ("gpio", "enable gpio control microservice")
                          ("status", "enable status microservice")
                          ("sensor", "enable sensor microservice")
                          ("sysid", boost::program_options::value<int>()->required(), "local mavlink system id");

        boost::program_options::variables_map vm;
        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
        boost::program_options::notify(vm);

        if (vm.count("help")) {
            std::cout << desc << std::endl;
            exit(0);
        }

        PlatformType platform_type = PlatformTypeUnknown;

        try {
            std::ifstream f("/tmp/platform_manifest");
            nlohmann::json j;
            f >> j;

            platform_type = platform_type_from_string(j["platform"]);
        } catch (std::exception &ex) {
            // don't do anything, but send an error message to the user through the status service
            std::cerr << "platform error: " << ex.what() << std::endl;
        }

        if (vm.count("camera")) {
            std::cout << "Camera microservice enabled ";
            service = new CameraMicroservice(io_service);
        } else if (vm.count("power")) {
            std::cout << "Power sensor microservice enabled ";
            service = new PowerMicroservice(io_service, platform_type);
        } else if (vm.count("gpio")) {
            std::cout << "GPIO control microservice enabled ";
            service = new GPIOMicroservice(io_service, platform_type);
        } else if (vm.count("status")) {
            std::cout << "Status microservice enabled ";
            service = new StatusMicroservice(io_service, platform_type);
        } else if (vm.count("sensor")) {
            std::cout << "Sensor sensor microservice enabled ";
            service = new SensorMicroservice(io_service, platform_type);
        } else {
            std::cout << "No service enabled" << std::endl;
            exit(1);
        }

        if (vm.count("sysid")) {
            auto sysid = vm["sysid"].as<int>();
            std::cout << "on sysid " << sysid << std::endl;
            service->set_sysid(sysid);
        } else {
            std::cout << "No sysid provided" << std::endl;
            exit(1);
        }
    } catch (std::exception &ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        exit(1);
    } catch (...) {
        std::cerr << "Unknown exception occurred" << std::endl;
        exit(1);
    }

    try {
        service->connect();
    } catch (std::exception &exception) {
        std::cout << "Connection failed: " << exception.what() << std::endl;
        exit(1);
    }

    try {
        service->setup();
    } catch (std::exception &exception) {
        std::cout << "Setup failed: " << exception.what() << std::endl;
        exit(1);
    }

    io_service.run();
    return 0;
}
