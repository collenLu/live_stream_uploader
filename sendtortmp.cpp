#include "sendtortmp.h"
#include <QString>
#include <QDebug>
extern "C"
{
    #include <libavformat/avformat.h>
    #include <libavutil/time.h>
}

class CSendToRtmp :public SendToRtmp
{
public:

    void Close()
    {
        if (ic)
        {
            avformat_close_input(&ic);
            vs = NULL;
        }
        vc = NULL;
        url = "";
    }
    bool Init(const char *url)
    {
        int ret = avformat_alloc_output_context2(&ic, 0, "flv", url);
        this->url = url;
        if (ret != 0)
        {
            char buf[1024] = { 0 };
            av_strerror(ret, buf, sizeof(buf) - 1);
            qDebug() <<buf;
            return false;
        }
        return true;
    }

    int AddStream(const AVCodecContext *c)
    {
        if (!c)return -1;

        AVStream *st = avformat_new_stream(ic, NULL);
        if (!st)
        {
            qDebug() << "avformat_new_stream failed";
            return -1;
        }
        st->codecpar->codec_tag = 0;

        avcodec_parameters_from_context(st->codecpar, c);

        if (c->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            vc = c;
            vs = st;
        }
        else if (c->codec_type == AVMEDIA_TYPE_AUDIO)
        {
            ac = c;
            as = st;
        }

        return st->index;
    }

    bool SendHead()
    {
        qDebug()<<"avio_open url "<<url.toLocal8Bit();

        int ret = avio_open(&ic->pb, url.toLocal8Bit(), AVIO_FLAG_WRITE);
        if (ret != 0)
        {
            char buf[1024] = { 0 };
            av_strerror(ret, buf, sizeof(buf) - 1);
            qDebug() << buf;
            return false;
        }

        ret = avformat_write_header(ic, NULL);
        if (ret != 0)
        {
            char buf[1024] = { 0 };
            av_strerror(ret, buf, sizeof(buf) - 1);
            qDebug() << buf;
            return false;
        }
        return true;
    }

    bool SendFrame(RawData d, int streamIndex)
    {
        if (!d.data || d.size <= 0 )return false;
        AVPacket *pack = (AVPacket *)d.data;
        pack->stream_index = streamIndex;
        AVRational stime;
        AVRational dtime;

        if (vs && vc&& pack->stream_index == vs->index)
        {
            stime = vc->time_base;
            dtime = vs->time_base;
        }
        else if (as && ac&&pack->stream_index == as->index)
        {
            stime = ac->time_base;
            dtime = as->time_base;
        }
        else
        {
            return false;
        }

        pack->pts = av_rescale_q(pack->pts, stime, dtime);
        pack->dts = av_rescale_q(pack->dts, stime, dtime);
        pack->duration = av_rescale_q(pack->duration, stime, dtime);
        int ret = av_interleaved_write_frame(ic, pack);
        if (ret == 0)
        {
            //qDebug() << "#" ;
            return true;
        }
        return false;
    }
private:
    //rtmp flv context
    AVFormatContext *ic = NULL;

    //video codec context
    const AVCodecContext *vc = NULL;

    //audio codec context
    const AVCodecContext *ac = NULL;

    //video stream
    AVStream *vs = NULL;

    //audio stream
    AVStream *as = NULL;

    QString url = "";
};

SendToRtmp * SendToRtmp::Get(unsigned char index)
{
    static CSendToRtmp cr[255];

    static bool isFirst = true;
    if (isFirst)
    {
        av_register_all();

        avformat_network_init();
        isFirst = false;
    }
    return &cr[index];
}

SendToRtmp::SendToRtmp()
{
}

SendToRtmp::~SendToRtmp()
{
}
