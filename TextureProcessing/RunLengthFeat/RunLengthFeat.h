#ifndef RUNLENGHFEAT_H
#define RUNLENGHFEAT_H

#include "Global.h"

using namespace std;

class RunLengthFeat
{

public:
    RunLengthFeat(InternalImageType::Pointer inputImage, InternalImageType::Pointer labelImage);
    ~RunLengthFeat();
    void Run();
    double GetFeatures(int i);
    double  features[11];

private:
    InternalImageType::Pointer featureImage;
    InternalImageType::Pointer labelImage;

};

#endif // RUNLENGHFEAT_H
