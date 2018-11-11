#include "controller.h"
#include "VideoCapture.h"
#include "AudioRecord.h"
#include "MediaEncode.h"
#include "SendtoRtmp.h"
#include <QDebug>

void Controller::run()
{
    long long beginTime = GetCurTime();
    while (!isExit)
    {
        RawData ad = AudioRecord::Get()->pop();
        RawData vd = MyVideoCapture::Get()->pop();
        if (ad.size<=0 && vd.size<=0)
        {
            msleep(1);
            continue;
        }

        if (ad.size > 0)
        {
            ad.pts = ad.pts - beginTime;

            RawData pcm = MediaEncode::Get()->Resample(ad);
            ad.drop();
            RawData pkt = MediaEncode::Get()->EncodeAudio(pcm);
            if (pkt.size > 0)
            {
                if (SendToRtmp::Get()->SendFrame(pkt,aindex))
                {
                    //qDebug() << "#";
                }
            }
        }

        if (vd.size > 0)
        {
            vd.pts = vd.pts - beginTime;
            RawData yuv = MediaEncode::Get()->RGBToYUV(vd);
            vd.drop();
            RawData pkt = MediaEncode::Get()->EncodeVideo(yuv);
            if (pkt.size> 0)
            {
                if (SendToRtmp::Get()->SendFrame(pkt,vindex))
                {
                    //qDebug() << "@" ;
                }
            }
        }
    }
}


bool Controller::Set(int val)
{
    FaceFilter::Get()->Set(val);
    return true;
}
bool Controller::Start()
{
    MyVideoCapture::Get()->AddFilter(FaceFilter::Get());
    qDebug() << "XController::Start()";

    if (camIndex >= 0)
    {
        if (!MyVideoCapture::Get()->Init(camIndex))
        {
            qDebug() << "MyVideoCapture init err";
            return false;
        }
    }
    else if (!inUrl.isEmpty())
    {
        if (!MyVideoCapture::Get()->InitWithUrl(inUrl.toLocal8Bit()))
        {
            err = "open";
            err += inUrl;
            err += "error";
            qDebug() << err;
            return false;
        }
    }
    else
    {
        err = "open url error";
        qDebug() << err << endl;
        return false;
    }
    qDebug() << "camera init success" << endl;

    if (!AudioRecord::Get()->Init())
    {
        err = "open audio device err";
        qDebug() << err ;
        return false;
    }
    qDebug() << "audio init success" << endl;

    AudioRecord::Get()->start();
    MyVideoCapture::Get()->start();

    MediaEncode::Get()->inWidth = MyVideoCapture::Get()->width;
    MediaEncode::Get()->inHeight = MyVideoCapture::Get()->height;
    MediaEncode::Get()->outWidth = MyVideoCapture::Get()->width;
    MediaEncode::Get()->outHeight = MyVideoCapture::Get()->height;
    if (!MediaEncode::Get()->InitScale())
    {
        err = "video init scale error!";
        qDebug() << err ;
        return false;
    }
    qDebug() << "video init scale success!";

    MediaEncode::Get()->channels = AudioRecord::Get()->channels;
    MediaEncode::Get()->nbSamples = AudioRecord::Get()->nbSamples;
    MediaEncode::Get()->sampleRate = AudioRecord::Get()->sampleRate;
    if (!MediaEncode::Get()->InitResample())
    {
        err = "aduio init resample error!";
        qDebug() << err << endl;
        return false;
    }
    qDebug() << "aduio init resample success!!" << endl;

    if (!MediaEncode::Get()->InitAudioCodec())
    {
        err = "init audiocode error!";
        qDebug() << err << endl;
        return false;
    }

    if (!MediaEncode::Get()->InitVideoCodec())
    {
        err = "init videocode error!";
        qDebug() << err << endl;
        return false;
    }
    qDebug() << "init video/audio codec success!" << endl;

    if (!SendToRtmp::Get()->Init(outUrl.toLocal8Bit()))
    {
        err = "init SendToRtmp error!";
        qDebug() << err << endl;
        return false;
    }
    qDebug() << "init SendToRtmp success!" << endl;

    vindex = SendToRtmp::Get()->AddStream(MediaEncode::Get()->vc);
    aindex = SendToRtmp::Get()->AddStream(MediaEncode::Get()->ac);
    if (vindex < 0 || aindex < 0)
    {
        err = "video index or audio index error!";
        qDebug() << err << endl;
        return false;
    }
    qDebug() << "Add video/audio stream success!" << endl;

    if (!SendToRtmp::Get()->SendHead())
    {
        err = "SendHead error!";
        qDebug() << err << endl;
        return false;
    }
    AudioRecord::Get()->clear();
    MyVideoCapture::Get()->clear();
    DataThread::start();
    return true;
}
void Controller::Stop()
{
    DataThread::stop();
    AudioRecord::Get()->Stop();
    MyVideoCapture::Get()->Stop();
    MediaEncode::Get()->Close();
    SendToRtmp::Get()->Close();
    camIndex = -1;
    inUrl = "";
    return;
}

Controller::Controller()
{
}

Controller::~Controller()
{
}

