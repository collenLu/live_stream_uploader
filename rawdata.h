#ifndef RAWDATA_H
#define RAWDATA_H


class RawData
{
public:
    char *data = 0;
    int size = 0;
    long long pts = 0;
    void drop();
    RawData();
    RawData(char *data,int size,long long pts=0);
    virtual ~RawData();
};
long long GetCurTime();

#endif // RAWDATA_H
