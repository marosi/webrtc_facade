#include "webrtc_facade.hpp"

#include "facade.hpp"

namespace xcs {
namespace nodes {

WebrtcFacade::WebrtcFacade() :
    facade_(new Facade()) {
    // empty
}

WebrtcFacade::~WebrtcFacade() {
    delete facade_;
}

void WebrtcFacade::start() {
    facade_->start();
}

void WebrtcFacade::setImageData(uint8_t *imageBytes, size_t len, int width, int height) {
    facade_->setImageData(imageBytes, len, width, height);
}

}
}

