#ifndef HISTOFEAT_H
#define HISTOFEAT_H

#include "Global.h"

using namespace std;

class HistoFeat
{

public:
    HistoFeat(InternalImageType::Pointer inputImage, InternalImageType::Pointer inputMap);
    ~HistoFeat();
    void Run();
    double GetFeatures(int i);
    double features[11];

private:
    InternalImageType::Pointer featureImage;
    InternalImageType::Pointer labelImage;

};

#endif // HISTOFEAT_H
