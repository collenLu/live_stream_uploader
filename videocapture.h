#ifndef VIDEOCAPTURE_H
#define VIDEOCAPTURE_H
#include "datathread.h"
#include "facefilter.h"
#include <QList>

class MyVideoCapture:public DataThread
{
public:
    int width = 0;
    int height = 0;
    int fps = 0;

    static MyVideoCapture *Get(unsigned char index = 0);
    virtual bool Init(int camIndex = 0) = 0;
    virtual bool InitWithUrl(const char *url) = 0;
    virtual void Stop() = 0;
    virtual ~MyVideoCapture();
    void AddFilter(FaceFilter *f)
    {
        fmutex.lock();
        filters.push_back(f);
        fmutex.unlock();
    }
protected:
    MyVideoCapture();
    QMutex fmutex;
    QList<FaceFilter*>filters;
};

#endif // VIDEOCAPTURE_H
