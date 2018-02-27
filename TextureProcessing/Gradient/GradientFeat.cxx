#include "GradientFeat.h"

#include "Global.h"

#include "itkImageFileWriter.h"
#include "itkLabelImageToStatisticsLabelMapFilter.hxx"
#include "itkGradientMagnitudeImageFilter.h"
#include "itkBinaryErodeImageFilter.h"
#include "itkBinaryBallStructuringElement.h"


GradientFeat::GradientFeat(InternalImageType::Pointer featureImage, InternalImageType::Pointer labelImage){
    this->featureImage = featureImage;
    this->labelImage = labelImage;
}

GradientFeat::~GradientFeat(){
    // Destructor. Not that necessary in our case.
}

void GradientFeat::Run(){

    typedef itk::GradientMagnitudeImageFilter<
              InternalImageType, InternalImageType >  GradFilterType;
    GradFilterType::Pointer gradFilter = GradFilterType::New();
    gradFilter->SetInput( this->featureImage );
    gradFilter->Update();

    float radius = 1.0;
    typedef itk::BinaryBallStructuringElement<
            InternalImageType::PixelType, Dimension> StructuringElementType;
    StructuringElementType structuringElement;
    structuringElement.SetRadius(radius);
    structuringElement.CreateStructuringElement();

    typedef itk::BinaryErodeImageFilter <InternalImageType, InternalImageType, StructuringElementType>
        BinaryErodeImageFilterType;
    BinaryErodeImageFilterType::Pointer erodeFilter = BinaryErodeImageFilterType::New();
    erodeFilter->SetInput(this->labelImage);
    erodeFilter->SetKernel(structuringElement);
    erodeFilter->SetBackgroundValue(0);
    erodeFilter->SetForegroundValue(1.0);
    erodeFilter->SetBoundaryToForeground(true);
    erodeFilter->Update();

    // Calculating the statistical parameters
    typedef itk::LabelImageToStatisticsLabelMapFilter< InternalImageType, InternalImageType > StatFilterType;
    StatFilterType::Pointer statFilter = StatFilterType::New();
    statFilter->SetFeatureImage( gradFilter->GetOutput() );
    statFilter->SetInput( erodeFilter->GetOutput() );
    statFilter->SetBackgroundValue(0);
    statFilter->Update();

    features[0] = statFilter->GetOutput()->GetLabelObject(1)->GetMean();
    features[1] = statFilter->GetOutput()->GetLabelObject(1)->GetStandardDeviation();
    features[2] = statFilter->GetOutput()->GetLabelObject(1)->GetVariance();
    features[3] = statFilter->GetOutput()->GetLabelObject(1)->GetMaximum();

    /*
    // Saving the Images
      typedef itk::ImageFileWriter<InternalImageType>  WriterType;
      typename WriterType::Pointer imageWriter = WriterType::New();

      imageWriter->SetInput( erodeFilter->GetOutput() );
      imageWriter->SetFileName("ErodelabelImage.nrrd");
      imageWriter->Update();

      imageWriter->SetInput( gradFilter->GetOutput() );
      imageWriter->SetFileName("GradfeatureImage.nrrd");
      imageWriter->Update();
    */

}

double GradientFeat::GetFeatures(int i){
    return features[i];
}
