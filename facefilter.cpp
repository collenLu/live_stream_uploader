#include "facefilter.h"
#include "bilateralfilter.h"

FaceFilter::FaceFilter()
{

}

FaceFilter* FaceFilter::Get(XFilterType t)
{
    static BilateralFilter bf;
    switch (t)
    {
    case XBILATERAL:
        return &bf;
        break;
    default:
        break;
    }
    return 0;
}

bool FaceFilter::Set(int value)
{
    faceLevel = value;
    return true;
}

FaceFilter::~FaceFilter()
{
}
