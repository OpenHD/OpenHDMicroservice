

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#include <iostream>
#include <stdexcept>
#include <vector>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include <gst/gst.h>

#include "gstreamerstream.h"



GStreamerStream::GStreamerStream(boost::asio::io_service &io_service, 
                                 PlatformType platform,
                                 Camera camera, 
                                 std::vector<CameraEndpoint> endpoints,
                                 uint16_t port)
    : CameraStream(io_service, platform, camera, endpoints, port) {
    std::cerr << "GStreamerStream::GStreamerStream()" << std::endl;

    setup();
    start();
}


void GStreamerStream::setup() {
    std::cerr << "GStreamerStream::setup()" << std::endl;

    GError* error = nullptr;

    if (!gst_init_check(nullptr, nullptr, &error)) {
        std::cerr << "gst_init_check() failed: " << error->message << std::endl;
        g_error_free(error);

        throw std::runtime_error("GStreamer initialization failed");
    }

    std::string device_node;

    // todo: introduce quirks system for specific cameras, if any need different behavior
    for (auto &endpoint : m_camera_endpoints) {
        if (endpoint.support_h264) {
            device_node = endpoint.device_node;
            break;
        }
    }

    std::stringstream pipeline;

    if (m_enable_videotest) {
        std::cerr << "Using video test" << std::endl;
        pipeline << "videotestsrc pattern=smpte ! ";
        pipeline << "video/x-raw,width=320,height=240 !";
    } else {
        std::cerr << "Creating GStreamer pipeline" << std::endl;

        switch (m_camera.type) {
            case CameraTypeRaspberryPiCSI: {
                pipeline << "rpicamsrc bitrate=5000000 preview=false name=bitratectrl camera-number=";
                pipeline << m_camera.bus;
                pipeline << " ! ";
                pipeline << "video/x-h264,profile=constrained-baseline,width=1280,height=720,framerate=48/1,level=3.0 !";
                break;
            }
            case CameraTypeJetsonCSI: {
                pipeline << "nvarguscamerasrc preview=false ";
                pipeline << m_camera.bus;
                pipeline << " ! ";
                pipeline << "video/x-raw(memory:NVMM), width=1280, height=720, format=NV12, framerate=48/1 !";
                pipeline << "queue !";
                pipeline << "nvv4l2h264enc name=bitratectrl control-rate=1 profile=1 insert-sps-pps=1 bitrate=5000000 !";
                break;              
            }
            case CameraTypeRockchipCSI: {
                break;
            }
            case CameraTypeUVC: {
                pipeline << "v4l2src device=";
                pipeline << device_node;
                pipeline << " ! ";
                pipeline << "video/x-h264, width=1280, height=720, framerate=30/1 !";
                break;
            }
            case CameraTypeIP: {
                pipeline << "rtspsrc location=\"";
                pipeline << device_node;
                pipeline << "\"";
                pipeline << "latency=0 !";
                break;
            }
            case CameraTypeUnknown: {
                std::cerr << "Unknown camera type" << std::endl;
                return;
            }
        }
    }
    pipeline << "queue !";        
    pipeline << "h264parse !";
    pipeline << "rtph264pay ! ";
    pipeline << "udpsink host=127.0.0.1 port=";
    pipeline << m_video_port;

    m_pipeline = gst_parse_launch(pipeline.str().c_str(), &error);
    if (error) {
        std::cerr << "Failed to create pipeline: " << error->message << std::endl;

        return;
    }


    //GstBus *bus = gst_pipeline_get_bus (GST_PIPELINE(m_pipeline));
    //gst_bus_add_signal_watch(bus);
    //g_signal_connect(bus, "message", (GCallback)PipelineCb, this);
}


void GStreamerStream::start() {
    std::cerr << "GStreamerStream::start()" << std::endl;
    
    gst_element_set_state(m_pipeline, GST_STATE_PLAYING);
}


void GStreamerStream::stop() {
    std::cerr << "GStreamerStream::stop()" << std::endl;

    gst_element_set_state(m_pipeline, GST_STATE_PAUSED);
}


bool GStreamerStream::supports_bitrate() {
    std::cerr << "GStreamerStream::supports_bitrate()" << std::endl;

    GstElement *bitratectrl = gst_bin_get_by_name(GST_BIN(m_pipeline), "bitratectrl");

    return (bitratectrl != nullptr);
}


void GStreamerStream::set_bitrate(int bitrate) {
    std::cerr << "GStreamerStream::set_bitrate(" << bitrate << ")" << std::endl;

    if (!m_pipeline) {
        std::cerr << "No pipeline, ignoring bitrate command" << std::endl;
        return;
    }

    // this depends on setting a particular pipeline element with name=bitratectrl, sometimes that will
    // be the source at the start of the pipeline, sometimes it will be an encoder element. Luckly all of them
    // use the same property name, "bitrate".
    GstElement *bitratectrl = gst_bin_get_by_name(GST_BIN(m_pipeline), "bitratectrl");

    if (!bitratectrl) {
        std::cerr << "No bitrate control element, ignoring bitrate command" << std::endl;
        return;
    }

    // this can fail, but it shouldn't affect anything if it does
    g_object_set(bitratectrl, "bitrate", bitrate, NULL);
}


bool GStreamerStream::supports_cbr() {
    std::cerr << "GStreamerStream::supports_cbr()" << std::endl;

    return m_camera.type == CameraTypeRaspberryPiCSI || m_camera.type == CameraTypeJetsonCSI;
}


void GStreamerStream::set_cbr(bool enable) {
    std::cerr << "GStreamerStream::set_cbr(" << enable << ")" << std::endl;

    if (!m_pipeline) {
        std::cerr << "No pipeline, ignoring bitrate command" << std::endl;
        return;
    }

    // this depends on setting a particular pipeline element with name=bitratectrl, sometimes that will
    // be the source at the start of the pipeline, sometimes it will be an encoder element. Luckly all of them
    // use the same property name, "bitrate".
    GstElement *bitratectrl = gst_bin_get_by_name(GST_BIN(m_pipeline), "bitratectrl");

    if (!bitratectrl) {
        std::cerr << "No bitrate control element, ignoring CBR/VBR command" << std::endl;
        return;
    }

    switch (m_camera.type) {
        case CameraTypeRaspberryPiCSI: {
            break;
        }
        case CameraTypeJetsonCSI: {
            g_object_set(bitratectrl, "control-rate", enable ? 1 : 0, NULL);
            break;
        }
        default: {
            break;
        }
    }
}


std::vector<std::string> GStreamerStream::get_supported_formats() {
    std::cerr << "GStreamerStream::get_supported_formats()" << std::endl;

    std::vector<std::string> formats;

    // todo: this should not always pick the h264 endpoint, there may not be one and some configurations may
    // use another endpoint even when an h264 endpoint exists
    for (auto &endpoint : m_camera_endpoints) {
        if (endpoint.support_h264) {
            formats = endpoint.formats;
        }
    }

    return formats;
}


std::string GStreamerStream::get_format() {
    std::cerr << "GStreamerStream::get_format()" << std::endl;
}


void GStreamerStream::set_format(std::string format) {
    std::cerr << "GStreamerStream::set_format(" << format << ")" << std::endl;

}



