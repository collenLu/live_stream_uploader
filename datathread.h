#ifndef DATATHREAD_H
#define DATATHREAD_H
#include <QThread>
#include <QList>
#include <QMutex>
#include "rawdata.h"

class DataThread:public QThread
{
public:
    DataThread();
    virtual ~DataThread();

    int maxList = 100;

    virtual void push(RawData d);

    virtual RawData pop();

    virtual bool start();

    virtual void stop();

    virtual void clear();
protected:
    QList<RawData> dataList;
    int dataCount = 0;
    QMutex mutex;
    bool isExit = false;
};

#endif // DATATHREAD_H
