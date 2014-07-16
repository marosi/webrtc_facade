#ifndef WEBRTC_FACADE_HPP
#define WEBRTC_FACADE_HPP

#if defined _WIN32 || defined __CYGWIN__
  #ifdef BUILDING_DLL
    #ifdef __GNUC__
      #define DLL_PUBLIC __attribute__ ((dllexport))
    #else
      #define DLL_PUBLIC __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #else
    #ifdef __GNUC__
      #define DLL_PUBLIC __attribute__ ((dllimport))
    #else
      #define DLL_PUBLIC __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #endif
  #define DLL_LOCAL
#else
  #if __GNUC__ >= 4
    #define DLL_PUBLIC __attribute__ ((visibility ("default")))
    #define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
  #else
    #define DLL_PUBLIC
    #define DLL_LOCAL
  #endif
#endif


#include <stddef.h>
#include <stdint.h>

namespace xcs {
namespace nodes {

class Facade;

class WebrtcFacade {

public:
    DLL_PUBLIC
    WebrtcFacade();
    DLL_PUBLIC
    ~WebrtcFacade();
    DLL_PUBLIC
    void start();
    DLL_PUBLIC
    void setImageData(uint8_t *imageBytes, size_t len, int width, int height);

private:
    Facade* facade_;
};

}
}


#endif // WEBRTC_FACADE_HPP
