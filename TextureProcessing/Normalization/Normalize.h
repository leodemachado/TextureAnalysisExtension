#ifndef NORMALIZE_H
#define NORMALIZE_H

#include "Global.h"

using namespace std;

class Normalize
{

public:
    Normalize(InternalImageType::Pointer inputImage, InternalImageType::Pointer labelImage);
    ~Normalize();
    void Run();
    InternalImageType::Pointer GetNormalized(int i);

private:
    InternalImageType::Pointer featureImage;
    InternalImageType::Pointer labelImage;
    InternalImageType::Pointer normlabelImage;
    InternalImageType::Pointer normFeatureImage;
};

#endif // NORMALIZE_H
