#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "datathread.h"
#include <QString>

class Controller:public DataThread
{
public:
    int camIndex = -1;
    QString outUrl = "";
    QString inUrl = "";
    QString err = "";

    static Controller *Get()
    {
        static Controller xc;
        return &xc;
    }

    virtual bool Set(int val);
    virtual bool Start();
    virtual void Stop();
    void run();
    virtual ~Controller();

protected:
    Controller();
    int vindex = 0; //video stream index
    int aindex = 1; //audio stream index
};

#endif // CONTROLLER_H
