#include "HistoFeat.h"

#include <itkImageRegionIterator.h>

#include "itkLabelImageToStatisticsLabelMapFilter.h"

#include <iostream>
#include <sstream>
//#include <ofstream.h>
#include <sys/stat.h>

HistoFeat::HistoFeat(InternalImageType::Pointer featureImage, InternalImageType::Pointer labelImage){
    this->featureImage = featureImage;
    this->labelImage = labelImage;

}

//int d = Dimension;

HistoFeat::~HistoFeat(){
    // Destructor. Not that necessary in our case.
}

void HistoFeat::Run(){

    // Calculating the statistical parameters
    typedef itk::LabelImageToStatisticsLabelMapFilter< InternalImageType, InternalImageType > FeatFilterType;
    typename FeatFilterType::Pointer featFilter = FeatFilterType::New();
    featFilter->SetInput( this->labelImage );
    featFilter->SetFeatureImage( this->featureImage );
    featFilter->SetBackgroundValue( 0 ); // Label background Value
    featFilter->Update();

    features[0] = featFilter->GetOutput()->GetLabelObject(1)->GetMean();
    features[1] = featFilter->GetOutput()->GetLabelObject(1)->GetStandardDeviation();
    features[2] = featFilter->GetOutput()->GetLabelObject(1)->GetMedian();
    features[3] = featFilter->GetOutput()->GetLabelObject(1)->GetSkewness();
    features[4] = featFilter->GetOutput()->GetLabelObject(1)->GetKurtosis();
    features[5] = featFilter->GetOutput()->GetLabelObject(1)->GetSum();
    features[6] = featFilter->GetOutput()->GetLabelObject(1)->GetVariance();
    features[7] = featFilter->GetOutput()->GetLabelObject(1)->GetWeightedElongation();
    features[8] = featFilter->GetOutput()->GetLabelObject(1)->GetWeightedFlatness();
    features[9] = featFilter->GetOutput()->GetLabelObject(1)->GetMinimum();
    features[10] = featFilter->GetOutput()->GetLabelObject(1)->GetMaximum();

    /*
    //std::cout << "Center Of Gravity " <<featFilter->GetOutput()->GetNumber<<std::endl;
    std::cout << "Principal Moments " <<featFilter->GetOutput()->GetLabelObject(1)->GetWeightedPrincipalMoments()<<std::endl;
    std::cout << "Principal Axes " <<featFilter->GetOutput()->GetLabelObject(1)->GetWeightedPrincipalAxes()<<std::endl;
    */
}

double HistoFeat::GetFeatures(int i){
    return features[i];
}

