webrtc_facade
=============

Library for "real-time" video streaming to browser (built on top WebRTC).


1) Clone repo in webrtc/trunk

2) To webrtc/trunk/all.gyp add line:
  '<(libjingle_root)/webrtc_facade/webrtc_facade.gyp:*',
  
3) gclient runhooks

4) ninja -C out/Release webrtc_facade
