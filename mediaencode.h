#ifndef MEDIAENCODE_H
#define MEDIAENCODE_H
#include "rawdata.h"

class AVCodecContext;
enum XSampleFMT
{
    X_S16 = 1,
    X_FLATP =8
};

class MediaEncode
{
public:
    //input parameter
    int inWidth = 1280;
    int inHeight = 720;
    int inPixSize = 3;
    int channels = 2;
    int sampleRate = 44100;
    XSampleFMT inSampleFmt = X_S16;

    //output parameter
    int outWidth = 1280;
    int outHeight = 720;
    int bitrate = 4000000;//50kB
    int fps = 25;
    int nbSamples = 1024;
    XSampleFMT outSampleFmt = X_FLATP;

    static MediaEncode * Get(unsigned char index = 0);

    virtual bool InitScale() = 0;

    virtual bool InitResample() = 0;

    virtual RawData Resample(RawData d) = 0;

    virtual RawData RGBToYUV(RawData rgb) = 0;

    virtual bool InitVideoCodec() = 0;

    virtual bool InitAudioCodec() = 0;

    virtual RawData EncodeVideo(RawData frame) = 0;

    virtual RawData EncodeAudio(RawData frame) = 0;

    virtual void Close() = 0;

    virtual ~MediaEncode();

    AVCodecContext *vc = 0;	//video context
    AVCodecContext *ac = 0; //audio context
protected:
    MediaEncode();
};

#endif // MEDIAENCODE_H
