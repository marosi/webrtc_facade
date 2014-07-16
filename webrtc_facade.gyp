{    
    'includes': 
    [
        '../talk/build/common.gypi',
    ],

    'targets':
    [{
        'target_name': 'webrtc_facade',
        'type': 'shared_library',
        'include_dirs' : 
        [
            # nothing
        ],
        'defines':
        [
            # nothing
        ],
        'cflags':
        [
            '-std=c++11',
        ],
        'ldflags':
        [
            '-v',
        ],
        'sources': 
        [
            'facade.cpp',
            'video_capturer.cpp',
            'webrtc_facade.cpp',
            
        ],
        'dependencies': 
        [
            '<(DEPTH)/third_party/jsoncpp/jsoncpp.gyp:jsoncpp',
            '<(DEPTH)/talk/libjingle.gyp:libjingle_peerconnection',
        ],
    }], # end targets
}
