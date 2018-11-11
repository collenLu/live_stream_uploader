#ifndef SENDTORTMP_H
#define SENDTORTMP_H
#include "rawdata.h"
class AVCodecContext;
class AVPacket;

class SendToRtmp
{
public:

    static SendToRtmp * Get(unsigned char index = 0);

    virtual bool Init(const char *url) = 0;

    virtual int AddStream(const AVCodecContext *c) = 0;

    virtual bool SendHead() = 0;

    virtual bool SendFrame(RawData d,int streamIndex = 0) = 0;

    virtual void Close() = 0;

    virtual ~SendToRtmp();
protected:
    SendToRtmp();
};

#endif // SENDTORTMP_H
