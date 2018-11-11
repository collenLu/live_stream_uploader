#ifndef BILATERALFILTER_H
#define BILATERALFILTER_H

#include "facefilter.h"

class BilateralFilter:public FaceFilter
{
public:
    BilateralFilter();
    bool Filter(cv::Mat *src, cv::Mat *des);
    virtual ~BilateralFilter();
};

#endif // BILATERALFILTER_H
