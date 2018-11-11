#include "videocapture.h"
#include <QDebug>
#include <opencv2/opencv.hpp>

using namespace cv;

class CMyVideoCapture :public MyVideoCapture
{
public:
    VideoCapture cam;

    void run()
    {
        Mat frame;
        while (!isExit)
        {
            if (!cam.read(frame))
            {
                msleep(1);
                continue;
            }
            if (frame.empty())
            {
                msleep(1);
                continue;
            }

            fmutex.lock();
            for (int i = 0; i < filters.size(); i++)
            {
                Mat des;
                filters[i]->Filter(&frame, &des);
                frame = des;
            }
            fmutex.unlock();

            RawData d((char*)frame.data, frame.cols*frame.rows*frame.elemSize(),GetCurTime());
            push(d);
        }
    }

    bool Init(int camIndex = 0)
    {
        cam.open(camIndex);
        if (!cam.isOpened())
        {
            qDebug()<< "cam open failed!" << endl;
            return false;
        }
        qDebug() << camIndex << " cam open success" << endl;
        width= cam.get(CAP_PROP_FRAME_WIDTH);
        height = cam.get(CAP_PROP_FRAME_HEIGHT);
        fps = cam.get(CAP_PROP_FPS);
        if (fps == 0) fps = 25;
        return true;
    }
    bool InitWithUrl(const char *url)
    {
        cam.open(url);
        if (!cam.isOpened())
        {
            qDebug() << "cam open failed!" << endl;
            return false;
        }
        qDebug() << url << " cam open success" << endl;
        width = cam.get(CAP_PROP_FRAME_WIDTH);
        height = cam.get(CAP_PROP_FRAME_HEIGHT);
        fps = cam.get(CAP_PROP_FPS);
        if (fps == 0) fps = 25;
        return true;
    }

    void Stop()
    {
        DataThread::stop();
        if (cam.isOpened())
        {
            cam.release();
        }
    }
};

MyVideoCapture* MyVideoCapture::Get(unsigned char index)
{
    static CMyVideoCapture xc[255];
    return &xc[index];
}

MyVideoCapture::MyVideoCapture()
{

}

MyVideoCapture::~MyVideoCapture()
{
}
