#ifndef FACADE_HPP
#define FACADE_HPP

#include <string>
#include <stddef.h>
#include <stdint.h>
#include "talk/app/webrtc/peerconnectioninterface.h"

namespace xcs {
namespace nodes {

class VideoCapturer;

class Facade {
public:
    Facade();
    ~Facade();
    void start();

    void setImageData(uint8_t *imageBytes, size_t len, int width, int height);
    void setLocalSessionDescription(std::string &sd);
    void setIceCandidate(std::string &candidate);

private:
    VideoCapturer* videoCapturer_;
    talk_base::scoped_refptr<webrtc::PeerConnectionFactoryInterface> pcFactory_;
    talk_base::scoped_refptr<webrtc::PeerConnectionInterface> pc_;
};

class PeerConnectionObserver : public webrtc::PeerConnectionObserver {
public:
    PeerConnectionObserver(Facade* facade);
    ~PeerConnectionObserver();

    void OnError();
    void OnStateChange(webrtc::PeerConnectionObserver::StateType state_changed);
    void OnAddStream(webrtc::MediaStreamInterface* stream);
    void OnRemoveStream(webrtc::MediaStreamInterface* stream);
    void OnRenegotiationNeeded();
    void OnIceChange();
    void OnIceCandidate(const webrtc::IceCandidateInterface* candidate);

private:
    Facade* facade_;
};

class SessionDescriptionObserver : public talk_base::RefCountedObject<webrtc::CreateSessionDescriptionObserver> {
public:
    SessionDescriptionObserver(Facade* facade);
    ~SessionDescriptionObserver();

    void OnSuccess(webrtc::SessionDescriptionInterface* desc);
    void OnFailure(const std::string& error);

private:
    Facade* facade_;
};

}
}


#endif // FACADE_HPP
