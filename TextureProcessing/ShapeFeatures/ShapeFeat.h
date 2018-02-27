#ifndef SHAPEFEAT_H
#define SHAPEFEAT_H

#include "Global.h"

using namespace std;

class ShapeFeat
{

public:
    ShapeFeat( InternalImageType::Pointer inputMap );
    ~ShapeFeat();
    void Run();
    double GetFeatures(int i);
    double features[9];

private:
    InternalImageType::Pointer labelImage;

};

#endif // SHAPEFEAT_H
