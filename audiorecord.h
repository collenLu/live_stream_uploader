#ifndef AUDIORECORD_H
#define AUDIORECORD_H
#include "datathread.h"


enum XAUDIOTYPE
{
    X_AUDIO_QT
};

class AudioRecord:public DataThread
{
public:
    int channels = 2;
    int sampleRate = 44100;
    int sampleByte = 2;
    int nbSamples = 1024;

    virtual bool Init() = 0;

    virtual void Stop() = 0;

    static AudioRecord *Get(XAUDIOTYPE type = X_AUDIO_QT, unsigned char index = 0);
    virtual ~AudioRecord();
protected:
    AudioRecord();
};

#endif // AUDIORECORD_H
