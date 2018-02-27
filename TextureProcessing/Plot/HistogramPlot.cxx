#include "HistogramPlot.h"

#include "Global.h"
//#include <TextureProcessing/Global.h>
#include <itkImageRegionIterator.h>

#include <iostream>
//#include <ofstream.h>


HistogramPlot::HistogramPlot(InternalImageType::Pointer featureImage, InternalImageType::Pointer labelImage){
    this->featureImage = featureImage;
    this->labelImage = labelImage;
    this->freq = new int[histoSize];
}

HistogramPlot::~HistogramPlot(){
    // Destructor. Not that necessary in our case.
}


void HistogramPlot::Run(){

    // Defining Iterator Types
    typedef itk::ImageRegionIterator< InternalImageType > IteratorType;

    // Create and instantiate the iterators: Const and NonConst
    IteratorType imageIter( featureImage, featureImage->GetRequestedRegion() );
    IteratorType labelIter( labelImage, labelImage->GetRequestedRegion() );

    for (int i = 0; i<histoSize; i++){
        freq[i] = 0;
    }


    for ( imageIter.GoToBegin(), labelIter.GoToBegin(); !imageIter.IsAtEnd();
          ++imageIter, ++labelIter ){

        if (labelIter.Get() != 0){
            int level = imageIter.Get();
            freq[level] = freq[level] + 1;
        }
     }
}

int HistogramPlot::GetHistogram(int i){
    return freq[i];
}
