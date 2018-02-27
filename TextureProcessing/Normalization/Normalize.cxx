#include "Normalize.h"

#include "Global.h"

#include "itkImageRegionIterator.h"
#include "itkLabelImageToStatisticsLabelMapFilter.h"

#include <iostream>
//#include <ofstream.h>


Normalize::Normalize(InternalImageType::Pointer featureImage, InternalImageType::Pointer labelImage){
    this->featureImage = featureImage;
    this->labelImage = labelImage;

    this->normlabelImage = InternalImageType::New();
    InternalImageType::RegionType Region = this->labelImage->GetRequestedRegion();
    this->normlabelImage->CopyInformation( this->labelImage );
    this->normlabelImage->SetRegions(Region);
    this->normlabelImage->Allocate();
    this->normlabelImage->FillBuffer(0);

    this->normFeatureImage = InternalImageType::New();
    Region = this->featureImage->GetRequestedRegion();
    this->normFeatureImage->CopyInformation( this->featureImage );
    this->normFeatureImage->SetRegions(Region);
    this->normFeatureImage->Allocate();
    this->normFeatureImage->FillBuffer(0);
}

Normalize::~Normalize(){
    // Destructor. Not that necessary in our case.
}

void Normalize::Run(){

    typedef itk::ImageRegionIterator< InternalImageType > IteratorType;

    // Calculating the statistical parameters
    typedef itk::LabelImageToStatisticsLabelMapFilter< InternalImageType, InternalImageType > StatFilterType;
    StatFilterType::Pointer statFilter = StatFilterType::New();
    statFilter->SetFeatureImage( this->featureImage );
    statFilter->SetInput( this->labelImage );
    statFilter->SetBackgroundValue(0); // Label background Value
    statFilter->Update();

    // Parameter to be sent to HistoNorm method
    const double mean = statFilter->GetOutput()->GetLabelObject(1)->GetMean();
    const double stDev = statFilter->GetOutput()->GetLabelObject(1)->GetStandardDeviation();
    const double lowerLimit = mean - 3.0*stDev;
    const double upperLimit = mean + 3.0*stDev;

    // Create and instantiate the iterators
    IteratorType imageIter( this->featureImage, this->featureImage->GetRequestedRegion() );
    IteratorType labelIter( this->labelImage, this->labelImage->GetRequestedRegion() );
    IteratorType normlabelIter( this->normlabelImage, this->normlabelImage->GetRequestedRegion() );

    // Applying routine to fill a new label with the normalized graylevel pixels
    for ( imageIter.GoToBegin(), labelIter.GoToBegin(), normlabelIter.GoToBegin(); !imageIter.IsAtEnd();
          ++imageIter, ++labelIter, ++normlabelIter ){
        if (labelIter.Get() != 0){
            int level = imageIter.Get();
            if ( level >= lowerLimit && level <= upperLimit) {
                normlabelIter.Set(1);
            }
        }
    }

    // After this point we a label that includes only the pixels in the desired range
    // Now we will buid a new image for receiving the rescaled values from the original range to (1 - 2^k), para k = 6 bits

    double min = statFilter->GetOutput()->GetLabelObject(1)->GetMinimum();
    double max = statFilter->GetOutput()->GetLabelObject(1)->GetMaximum();
    double alfa = (normRange-1)/(max-min);

    // alfa = (outputMax - outputMin)/(inputMax - inputMin) - Here is 256 - 1 = 255!

    IteratorType normImageIter( this->normFeatureImage, this->normFeatureImage->GetRequestedRegion() );

    // Applying routine to fill a new label with the normalized graylevel pixels
    for ( imageIter.GoToBegin(), normlabelIter.GoToBegin(), normImageIter.GoToBegin(); !imageIter.IsAtEnd();
          ++imageIter, ++normlabelIter, ++normImageIter ){
        if (normlabelIter.Get() != 0){
            int level = imageIter.Get();
            level = alfa*( level - min ) + 1.0;
            normImageIter.Set(level);
        }
    }
    // **End of normalization rotine
    // //////////////////////////////////////////////////////////////////////////////////////////////////////
}

InternalImageType::Pointer Normalize::GetNormalized(int i){
    if (i==0){
        return this->normFeatureImage;
    }
    else if (i==1){
        return this->normlabelImage;
    }
}
