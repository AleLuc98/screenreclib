//
// Created by giuli on 27/12/2021.
//

#ifndef SCREENRECLIB_SCPPTOOLS_H
#define SCREENRECLIB_SCPPTOOLS_H

#endif //SCREENRECLIB_SCPPTOOLS_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <cmath>
#include <cstring>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include "exceptions/exceptions.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavcodec/avfft.h>
#include <libavdevice/avdevice.h>
#include <libavfilter/avfilter.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavformat/avformat.h>
#include <libavformat/avio.h>

#include "libavutil/opt.h"
#include "libavutil/common.h"
#include "libavutil/channel_layout.h"
#include "libavutil/imgutils.h"
#include "libavutil/mathematics.h"
#include "libavutil/samplefmt.h"
#include "libavutil/time.h"
#include "libavutil/opt.h"
#include "libavutil/pixdesc.h"
#include "libavutil/file.h"
#include "libavutil/audio_fifo.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
}


using namespace  std;

typedef struct S{
    int width;
    int height;
}SRResolution;

typedef struct T{
    int x;
    int y;
}SROffset;

typedef struct A{
    bool _recaudio;
    bool _recvideo;
    SRResolution  _inscreenres;
    SRResolution  _outscreenres;
    SROffset _screenoffset;
    uint16_t  _fps;
    char* filename;
}SRSettings;

