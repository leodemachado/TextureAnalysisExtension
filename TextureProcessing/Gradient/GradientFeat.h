#ifndef GRADIENTFEAT_H
#define GRADIENTFEAT_H

#include "Global.h"

using namespace std;

class GradientFeat
{

public:
    GradientFeat(InternalImageType::Pointer featureImage, InternalImageType::Pointer labelImage);
    ~GradientFeat();
    void Run();
    double GetFeatures(int i);
    double features[4];

private:
    InternalImageType::Pointer featureImage;
    InternalImageType::Pointer labelImage;

};

#endif // GRADIENTFEAT_H
