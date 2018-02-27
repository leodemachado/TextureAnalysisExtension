#ifndef COOCURRENCEFEAT_H
#define COOCURRENCEFEAT_H

#include "Global.h"

using namespace std;

class CoocurrenceFeat
{

public:
    CoocurrenceFeat(InternalImageType::Pointer inputImage, InternalImageType::Pointer inputMap);
    ~CoocurrenceFeat();
    void Run();
    double GetFeatures(int i);
    double features[8];

private:
    InternalImageType::Pointer featureImage;
    InternalImageType::Pointer labelImage;

};

#endif // COOCURRENCEFEAT_H
