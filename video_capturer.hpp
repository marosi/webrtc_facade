#ifndef VIDEO_CAPTURER_HPP
#define VIDEO_CAPTURER_HPP

#include <cstdint>
#include "talk/media/base/videocapturer.h"

namespace xcs {
namespace nodes {

class VideoCapturer : public cricket::VideoCapturer {
public:
    VideoCapturer();

    bool GetBestCaptureFormat(const cricket::VideoFormat& desired, cricket::VideoFormat* best_format);
    cricket::CaptureState Start(const cricket::VideoFormat& capture_format);
    void Stop();
    bool IsRunning();
    bool IsScreencast() const;
    bool GetPreferredFourccs(std::vector<uint32>* fourccs);

    void setImageData(uint8_t *pImageBytes, size_t len, int width, int height);

private:
    bool is_running_;

};

}
}


#endif // VIDEO_CAPTURER_HPP
