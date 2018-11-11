#ifndef FACEFILTER_H
#define FACEFILTER_H
#include <QString>
#include <map>
namespace cv
{
class Mat;
}

enum XFilterType
{
    XBILATERAL
};

class FaceFilter
{
public:
    static FaceFilter * Get(XFilterType t = XBILATERAL);
    virtual bool Filter(cv::Mat *src, cv::Mat *des) = 0 ;
    virtual bool Set(int value);
    virtual ~FaceFilter();
protected:
    int faceLevel = 0;
    FaceFilter();
};

#endif // FACEFILTER_H
