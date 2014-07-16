#include "facade.hpp"

#include "video_capturer.hpp"

#include "talk/app/webrtc/test/fakeconstraints.h"
#include "talk/app/webrtc/videosourceinterface.h"

#include <iostream>

using namespace std;

namespace xcs {
namespace nodes {

class DummySetSessionDescriptionObserver : public webrtc::SetSessionDescriptionObserver {
public:
        static DummySetSessionDescriptionObserver* Create() {
            return new talk_base::RefCountedObject<DummySetSessionDescriptionObserver>();
        }

        void OnSuccess() {
            cerr << "DummySetSessionDescriptionObserver: onSuccess" << endl;
        }

        void OnFailure(const std::string& error) {
            cerr << "DummySetSessionDescriptionObserver::OnFailure" << endl;
        }

protected:
        DummySetSessionDescriptionObserver() {}
        ~DummySetSessionDescriptionObserver() {}
};

Facade::Facade() :
    videoCapturer_(new VideoCapturer()),
    pcFactory_(0) {

}

Facade::~Facade() {
    delete videoCapturer_;
}

void Facade::start() {
    if (pcFactory_ != 0)
    {
            cerr << "onStartClicked: already exists" << endl;
            return;
    }

    pcFactory_ = webrtc::CreatePeerConnectionFactory();

    if (!pcFactory_.get()) // This means that the initialization failed for some reason (get retrieves the pointer)
    {
            pcFactory_ = 0; // Since it's ref counted, this will destroy it
            return;
    }

    webrtc::PeerConnectionInterface::IceServers servers;
    webrtc::PeerConnectionInterface::IceServer server;
    webrtc::FakeConstraints constraints;

    constraints.SetAllowDtlsSctpDataChannels();

    server.uri = "stun:stun.l.google.com:19302";
    servers.push_back(server);

    // TODO: this is a memory leak for now
    PeerConnectionObserver* pcObserver = new PeerConnectionObserver(this);

    pc_ = pcFactory_->CreatePeerConnection(servers, &constraints, 0, 0, pcObserver);

    talk_base::scoped_refptr<webrtc::VideoSourceInterface> videoSource;
    videoSource = pcFactory_->CreateVideoSource(videoCapturer_, 0);

    talk_base::scoped_refptr<webrtc::VideoTrackInterface> videoTrack;
    videoTrack = pcFactory_->CreateVideoTrack("testVideoTrack", videoSource);

    talk_base::scoped_refptr<webrtc::MediaStreamInterface> stream;
    stream = pcFactory_->CreateLocalMediaStream("testStream");

    stream->AddTrack(videoTrack);

    if (!pc_->AddStream(stream, 0))
    {
            cerr << "Adding stream to PeerConnection failed" << endl;
            return;
    }
    cerr << "Successfully added stream" << endl;

    SessionDescriptionObserver* sessionDescriptionObserver = new SessionDescriptionObserver(this);

    pc_->CreateOffer(sessionDescriptionObserver, 0);

    // REMOTE
    cerr << ">>> Awaiting remote SDP info >>>" << endl;
    string remoteSdpInfo;
    cin >> remoteSdpInfo;

    webrtc::SessionDescriptionInterface* remoteSessionDescription = webrtc::CreateSessionDescription("answer", remoteSdpInfo);
    pc_->SetRemoteDescription(DummySetSessionDescriptionObserver::Create(), remoteSessionDescription);

    cerr << ">>>Awaiting remote ICE candidate >>>" << endl;
    string remoteIceCandidate;
    cin >> remoteIceCandidate;
}

void Facade::setImageData(uint8_t *imageBytes, size_t len, int width, int height) {
    videoCapturer_->setImageData(imageBytes, len, width, height);
}

void Facade::setLocalSessionDescription(string &sd) {
    webrtc::SessionDescriptionInterface* pSessionDescription = webrtc::CreateSessionDescription("offer", sd);
    pc_->SetLocalDescription(DummySetSessionDescriptionObserver::Create(), pSessionDescription);
}

void Facade::setIceCandidate(string &candidate) {

}

/*
 * PeerConnectionObserver
 */
PeerConnectionObserver::PeerConnectionObserver(Facade* facade) :
    facade_(facade) {
        cerr << "PeerConnectionObserver::PeerConnectionObserver" << endl;
}

PeerConnectionObserver::~PeerConnectionObserver() {
        cerr << "PeerConnectionObserver::~PeerConnectionObserver" << endl;
}

void PeerConnectionObserver::OnError() {
        cerr << "PeerConnectionObserver::OnError" << endl;
}

void PeerConnectionObserver::OnStateChange(webrtc::PeerConnectionObserver::StateType state_changed) {
        cerr << "PeerConnectionObserver::OnStateChange" << endl;
}

void PeerConnectionObserver::OnAddStream(webrtc::MediaStreamInterface* stream) {
        cerr << "PeerConnectionObserver::OnAddStream" << endl;
}

void PeerConnectionObserver::OnRemoveStream(webrtc::MediaStreamInterface* stream) {
        cerr << "PeerConnectionObserver::OnRemoveStream" << endl;
}

void PeerConnectionObserver::OnRenegotiationNeeded() {
        cerr << "PeerConnectionObserver::OnRenegotiationNeeded" << endl;
}

void PeerConnectionObserver::OnIceChange() {
        cerr << "PeerConnectionObserver::OnIceChange" << endl;
}

void PeerConnectionObserver::OnIceCandidate(const webrtc::IceCandidateInterface* candidate) {
        cerr << "PeerConnectionObserver::OnIceCandidate" << endl;
        string iceStr;

        int mlineIdx = candidate->sdp_mline_index();
        string sdpMid = candidate->sdp_mid();
        candidate->ToString(&iceStr);

        size_t len = iceStr.length();
        if (len > 0 && iceStr[len-1] == '\n')
                iceStr = iceStr.substr(0, len-1);

        len = iceStr.length();
        if (len > 0 && iceStr[len-1] == '\r')
                iceStr = iceStr.substr(0, len-1);

#define BUFLEN 1024
        char buf[BUFLEN];

        snprintf(buf, BUFLEN, "{\"sdpMLineIndex\":%d,\"sdpMid\":\"%s\",\"candidate\":\"%s\\r\\n\"}", mlineIdx, sdpMid.c_str(), iceStr.c_str());

        // TODO
        cerr << buf << endl;
}

/*
 * SessionDescriptionObserver
 */
SessionDescriptionObserver::SessionDescriptionObserver(Facade* facade) :
    facade_(facade) {
        cerr << "SessionDescriptionObserver::SessionDescriptionObserver" << endl;
}

SessionDescriptionObserver::~SessionDescriptionObserver() {
        cerr << "SessionDescriptionObserver::~SessionDescriptionObserver" << endl;
}

void SessionDescriptionObserver::OnSuccess(webrtc::SessionDescriptionInterface* desc) {
        cerr << "SessionDescriptionObserver::OnSuccess" << endl;
        string sd;
        desc->ToString(&sd);
        facade_->setLocalSessionDescription(sd);

        cerr << sd << endl;

}

void SessionDescriptionObserver::OnFailure(const string& error) {
        cerr << "SessionDescriptionObserver::OnFailure" << endl;
}

}
}
