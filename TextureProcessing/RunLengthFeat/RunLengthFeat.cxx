#include "RunLengthFeat.h"

#include "itkDenseFrequencyContainer2.h"
#include "itkScalarImageToRunLengthMatrixFilter.h"
#include "itkHistogramToRunLengthFeaturesFilter.h"
#include "itkScalarImageToRunLengthFeaturesFilter.h"

#include "itkImageRegionIteratorWithIndex.h"
#include "itkBoundingBox.h"
#include "itkPointSet.h"
#include <itkNumberToString.h>

#include "itkLabelImageToStatisticsLabelMapFilter.h"

#include <iostream>
#include <sstream>
#include <sys/stat.h>
#include <itkNeighborhood.h>

RunLengthFeat::RunLengthFeat(InternalImageType::Pointer featureImage, InternalImageType::Pointer labelImage){
    this->featureImage = featureImage;
    this->labelImage = labelImage;
}

RunLengthFeat::~RunLengthFeat(){
    // Destructor. Not that necessary in our case.
}

void RunLengthFeat::Run(){

    // Calculating the statistical parameters
    typedef itk::LabelImageToStatisticsLabelMapFilter< InternalImageType, InternalImageType > StatFilterType;
    StatFilterType::Pointer statFilter = StatFilterType::New();
    statFilter->SetFeatureImage( this->labelImage );
    statFilter->SetInput( this->labelImage );
    statFilter->SetBackgroundValue(0); // Label background Value
    statFilter->Update();

    float numberOfPixels = statFilter->GetOutput()->GetLabelObject(1)->GetSum();

    // Defining types - Filter and Histogram Types
    typedef itk::Statistics::DenseFrequencyContainer2 HistogramFrequencyContainerType;
    typedef itk::Statistics::ScalarImageToRunLengthMatrixFilter
            <InternalImageType,HistogramFrequencyContainerType> RunLengthFilterType;
    typedef RunLengthFilterType::HistogramType HistogramType;
    typedef itk::Statistics::HistogramToRunLengthFeaturesFilter <HistogramType> FeaturesFilterType;
    RunLengthFilterType::Pointer runLengthFilter = RunLengthFilterType::New();

    runLengthFilter->SetInput( this->featureImage );
    runLengthFilter->SetMaskImage( this->labelImage );
    runLengthFilter->SetInsidePixelValue( 1 );

    // Defining Offsets and neighborhood
    typedef InternalImageType::OffsetType OffsetType;
    typedef itk::Neighborhood<float, Dimension> NeighborhoodType;
    NeighborhoodType neighborhood;

    // Filling offsetVec values with neghborhood iterator
    neighborhood.SetRadius(1);
    unsigned int centerIndex = neighborhood.GetCenterNeighborhoodIndex();
    typedef InternalImageType::OffsetType OffsetType;
    RunLengthFilterType::OffsetVectorPointer offsetVec= RunLengthFilterType::OffsetVector::New();

    for ( unsigned int d = 0; d < centerIndex; d++ )
    {
        OffsetType offset = neighborhood.GetOffset(d);
        offsetVec->push_back(offset);
    }


    // //////////////////////////////////////////////////////////////////
    /*
     * Bounding Box definition */
     itk::ImageRegionIteratorWithIndex<InternalImageType>
     ItI( this->featureImage, this->featureImage->GetLargestPossibleRegion());

      InternalPixelType maxValue = itk::NumericTraits<InternalPixelType>::NonpositiveMin();
      InternalPixelType minValue = itk::NumericTraits<InternalPixelType>::max();

      typedef itk::BoundingBox<unsigned long, Dimension, NumType> BoundingBoxType;
      BoundingBoxType::Pointer bbox = BoundingBoxType::New();
      BoundingBoxType::PointsContainerPointer points = BoundingBoxType::PointsContainer::New();
      itk::Point<NumType, Dimension> point;

      unsigned int idx = 0;

      for( ItI.GoToBegin(); !ItI.IsAtEnd(); ++ItI )
        {
        if ( !this->labelImage || ( this->labelImage->GetPixel( ItI.GetIndex() ) == 1 ) )
         {
          if ( ItI.Get() < minValue ) // Pixel value (grey level) statement
            {
            minValue = ItI.Get();
            }
          else if ( ItI.Get() > maxValue )
            {
            maxValue = ItI.Get();
            }
          this->featureImage->TransformIndexToPhysicalPoint( ItI.GetIndex(), point );
          points->InsertElement( idx++, point );
          }
        }
      bbox->SetPoints( points );
      bbox->ComputeBoundingBox();
      BoundingBoxType::PointType pointMin = bbox->GetMinimum();
      BoundingBoxType::PointType pointMax = bbox->GetMaximum();
      // BBOX end

    // Finishing the filter setting - Generating the run-length matrix
    runLengthFilter->SetOffsets(offsetVec);
    runLengthFilter->SetPixelValueMinMax(minValue, maxValue);
    runLengthFilter->SetNumberOfBinsPerAxis(normRange); //Default values
    runLengthFilter->SetDistanceValueMinMax( 0, pointMin.EuclideanDistanceTo(pointMax) );
    //runLengthFilter->FastCalculationsOff();
    runLengthFilter->Update();

    // Outputing features out of matrix
    FeaturesFilterType::Pointer histToFeatures = FeaturesFilterType::New();
    histToFeatures->SetInput(runLengthFilter->GetOutput());
    histToFeatures->Update();

    features[0] = histToFeatures->GetShortRunEmphasis();
    features[1] = histToFeatures->GetLongRunEmphasis();
    features[2] = histToFeatures->GetGreyLevelNonuniformity();
    features[3] = histToFeatures->GetRunLengthNonuniformity();
    features[4] = histToFeatures->GetLowGreyLevelRunEmphasis();
    features[5] = histToFeatures->GetHighGreyLevelRunEmphasis();
    features[6] = histToFeatures->GetShortRunLowGreyLevelEmphasis();
    features[7] = histToFeatures->GetShortRunHighGreyLevelEmphasis();
    features[8] = histToFeatures->GetLongRunLowGreyLevelEmphasis();
    features[9] = histToFeatures->GetLongRunHighGreyLevelEmphasis();
    features[10] = (histToFeatures->GetTotalNumberOfRuns())/(13.0*numberOfPixels);

}

double RunLengthFeat::GetFeatures(int i){
    return features[i];
}

