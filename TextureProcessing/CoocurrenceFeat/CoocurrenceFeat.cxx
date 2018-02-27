#include "CoocurrenceFeat.h"

#include "itkHistogramToTextureFeaturesFilter.h"
#include "itkScalarImageToCooccurrenceMatrixFilter.h"
#include "itkLabelImageToStatisticsLabelMapFilter.h"

#include <iostream>
#include <sstream>
//#include <ofstream.h>
#include <sys/stat.h>
#include <itkNeighborhood.h>


CoocurrenceFeat::CoocurrenceFeat(InternalImageType::Pointer featureImage, InternalImageType::Pointer labelImage){
    this->featureImage = featureImage;
    this->labelImage = labelImage;
}

CoocurrenceFeat::~CoocurrenceFeat(){
    // Destructor. Not that necessary in our case.
}

void CoocurrenceFeat::Run(){

    // Calculating maximum and minimum values in the label
    typedef itk::LabelImageToStatisticsLabelMapFilter< InternalImageType, InternalImageType > FeatFilterType;
    typename FeatFilterType::Pointer featFilter = FeatFilterType::New();
    featFilter->SetInput( this->labelImage );
    featFilter->SetFeatureImage( this->featureImage );
    featFilter->SetBackgroundValue( 0 ); // Label background Value
    featFilter->Update();

    // Creating the filter
    typedef itk::Statistics::ScalarImageToCooccurrenceMatrixFilter< InternalImageType > Image2CoocFilterType;
    Image2CoocFilterType::Pointer Image2CoocFilter = Image2CoocFilterType::New();

    // Defining offsetvec (counting directions) for cooccurrence filter
     Image2CoocFilterType::OffsetVectorPointer offsetVec = Image2CoocFilterType::OffsetVector::New();

     // Definitions used to fill the offsets
     typedef itk::Neighborhood<int, Dimension> NeighborhoodType;
     NeighborhoodType neighborhood;
     neighborhood.SetRadius(1);
     unsigned int centerIndex = neighborhood.GetCenterNeighborhoodIndex();
     typedef InternalImageType::OffsetType OffsetType;

     for ( unsigned int d = 0; d < centerIndex; d++ )
     {
         OffsetType offset = neighborhood.GetOffset(d);         
         offsetVec->push_back(offset);
     }

    // Filling and applying the Image2CoocFilter
    Image2CoocFilter->SetOffsets(offsetVec);
    Image2CoocFilter->SetPixelValueMinMax(featFilter->GetOutput()->GetLabelObject(1)->GetMinimum(),  // Specifica values
                                          featFilter->GetOutput()->GetLabelObject(1)->GetMaximum()); // From thw image
    Image2CoocFilter->SetNumberOfBinsPerAxis(normRange); // Will use the range after normalization
    Image2CoocFilter->SetInput( featureImage );
    Image2CoocFilter->SetMaskImage( labelImage );  // If not given, it will compute the whole image
    Image2CoocFilter->Update();

    typedef Image2CoocFilterType::HistogramType HistogramType;
    typedef itk::Statistics::HistogramToTextureFeaturesFilter<HistogramType> Hist2FeaturesType;
    Hist2FeaturesType::Pointer Histogram2FeaturesFilter = Hist2FeaturesType::New();
    Histogram2FeaturesFilter->SetInput( Image2CoocFilter->GetOutput() );
    Histogram2FeaturesFilter->Update();

    features[0] = Histogram2FeaturesFilter->GetFeature(Hist2FeaturesType::Entropy);
    features[1] = Histogram2FeaturesFilter->GetFeature(Hist2FeaturesType::Energy);
    features[2] = Histogram2FeaturesFilter->GetFeature(Hist2FeaturesType::Correlation);
    features[3] = Histogram2FeaturesFilter->GetFeature(Hist2FeaturesType::HaralickCorrelation);
    features[4] = Histogram2FeaturesFilter->GetFeature(Hist2FeaturesType::ClusterProminence);
    features[5] = Histogram2FeaturesFilter->GetFeature(Hist2FeaturesType::ClusterShade);
    features[6] = Histogram2FeaturesFilter->GetFeature(Hist2FeaturesType::Inertia);
    features[7] = Histogram2FeaturesFilter->GetFeature(Hist2FeaturesType::InverseDifferenceMoment);

}

double CoocurrenceFeat::GetFeatures(int i){
    return features[i];
}

