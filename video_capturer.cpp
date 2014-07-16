#include "video_capturer.hpp"

#include <iostream>
#include <chrono>

namespace xcs {
namespace nodes {

VideoCapturer::VideoCapturer() :
    is_running_(false) {
    // empty
}

bool VideoCapturer::GetBestCaptureFormat(const cricket::VideoFormat& desired, cricket::VideoFormat* best_format) {
    std::cerr << "TestVideoCapturer::GetBestCaptureFormat";
    *best_format = desired; // TODO
    return true;
}

cricket::CaptureState VideoCapturer::Start(const cricket::VideoFormat& capture_format) {
    is_running_ = true;
    return cricket::CS_RUNNING;
}

void VideoCapturer::Stop() {
    is_running_ = false;
}

bool VideoCapturer::IsRunning() {
    return is_running_;
}

bool VideoCapturer::IsScreencast() const {
    return true;
}

bool VideoCapturer::GetPreferredFourccs(std::vector<uint32>* fourccs) {
    fourccs->push_back(cricket::FOURCC_ABGR);
    return true;
}

void VideoCapturer::setImageData(uint8_t *pImageBytes, size_t len, int width, int height) {
    if (is_running_) {

        cricket::CapturedFrame frame;


        frame.elapsed_time = 33333333;

        std::chrono::nanoseconds dur = std::chrono::high_resolution_clock::now().time_since_epoch();
        frame.time_stamp = dur.count();
        //prevTimestamp += frame.elapsed_time;  // 30 fps

        frame.data = pImageBytes;
        frame.width = width;
        frame.height = height;
        frame.fourcc = cricket::FOURCC_ABGR;
        frame.data_size = len;

        if (frame.width*frame.height*4 != len) {
            std::cerr << "VideoCapturer: with and height don't match size for ABGR data, skipping frame";
        } else {
            //LOG(LS_INFO) << "CaptureThread: got frame " << (int)frameVal << " glGetError: " << glGetError();
            this->SignalFrameCaptured(this, &frame);
        }
    }
}

}
}


