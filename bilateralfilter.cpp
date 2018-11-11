#include "bilateralfilter.h"
#include <opencv2/opencv.hpp>

using namespace cv;

BilateralFilter::BilateralFilter()
{

}

bool BilateralFilter::Filter(cv::Mat *src, cv::Mat *des)
{
    double d = faceLevel;
    bilateralFilter(*src, *des, d, d * 2, d / 2);
    return true;
}

BilateralFilter::~BilateralFilter()
{

}
