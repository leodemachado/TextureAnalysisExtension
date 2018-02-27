#ifndef HISTOGRAMPLOT_H
#define HISTOGRAMPLOT_H

#include "Global.h"

using namespace std;

class HistogramPlot
{

public:
    HistogramPlot(InternalImageType::Pointer inputImage, InternalImageType::Pointer labelImage);
    ~HistogramPlot();
    void Run();
    int GetHistogram(int i);

private:
    InternalImageType::Pointer featureImage;
    InternalImageType::Pointer labelImage;
    int *freq;

};

#endif // HISTOGRAMPLOT_H
