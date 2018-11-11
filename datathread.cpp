#include "datathread.h"

DataThread::DataThread()
{

}

void DataThread::clear()
{
    mutex.lock();
    while (!dataList.empty())
    {
        dataList.front().drop();
        dataList.pop_front();
    }
    mutex.unlock();
}

void DataThread::push(RawData d)
{
    mutex.lock();
    if (dataList.size() > maxList)
    {
        dataList.front().drop();
        dataList.pop_front();
    }
    dataList.push_back(d);
    mutex.unlock();
}

RawData DataThread::pop()
{
    mutex.lock();
    if (dataList.empty())
    {
        mutex.unlock();
        return RawData();
    }
    RawData d = dataList.front();
    dataList.pop_front();
    mutex.unlock();
    return d;
}

bool DataThread::start()
{
    isExit = false;
    QThread::start();
    return true;
}

void DataThread::stop()
{
    isExit = true;
    wait();
}

DataThread::~DataThread()
{

}











