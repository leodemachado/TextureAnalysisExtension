// Header that carries the global type definitions and variables.
#ifndef GLOBAL_H
#define GLOBAL_H

#include <itkImage.h>

using namespace std;

static const int histoSize = 128;
static const int normRange = histoSize; // Used in normalization routune
static const int Dimension = 3;
typedef unsigned short InternalPixelType;
typedef float          NumType;
typedef itk::Image<InternalPixelType, Dimension> InternalImageType;

#endif // GLOBAL_H
