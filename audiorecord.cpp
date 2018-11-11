#include "audiorecord.h"
#include <QAudioInput>
#include <QDebug>

class CAudioRecord :public AudioRecord
{
public:
    void run()
    {
        int readSize = nbSamples*channels*sampleByte;
        char *buf = new char[readSize];
        while (!isExit)
        {
            if (input->bytesReady() < readSize)
            {
                QThread::msleep(1);
                continue;
            }

            int size = 0;
            while (size != readSize)
            {
                int len = io->read(buf + size, readSize - size);
                if (len < 0)break;
                size += len;
            }
            if (size != readSize)
            {
                continue;
            }
            long long pts = GetCurTime();

            push(RawData(buf, readSize, pts));
        }
        delete buf;
        qDebug() << "AudioRecord stop thead!" << endl;
    }

    bool Init()
    {
        Stop();
        QAudioFormat fmt;
        fmt.setSampleRate(sampleRate);
        fmt.setChannelCount(channels);
        fmt.setSampleSize(sampleByte * 8);
        fmt.setCodec("audio/pcm");
        fmt.setByteOrder(QAudioFormat::LittleEndian);
        fmt.setSampleType(QAudioFormat::UnSignedInt);
        QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
        if (!info.isFormatSupported(fmt))
        {
            qDebug() << "Audio format not support!";
            fmt = info.nearestFormat(fmt);
        }
        input = new QAudioInput(fmt);

        io = input->start();
        if (!io)
            return false;
        return true;
    }
    void Stop()
    {
        DataThread::stop();
        if (input)
            input->stop();
        if (io)
            io->close();
        input = NULL;
        io = NULL;
    }
    QAudioInput *input = NULL;
    QIODevice *io = NULL;
};


AudioRecord *AudioRecord::Get(XAUDIOTYPE type, unsigned char index)
{
    static CAudioRecord record[255];
    return &record[index];
}

AudioRecord::AudioRecord()
{
}

AudioRecord::~AudioRecord()
{
}

