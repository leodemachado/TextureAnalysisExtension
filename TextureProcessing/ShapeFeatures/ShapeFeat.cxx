#include "ShapeFeat.h"

#include "itkLabelImageToShapeLabelMapFilter.h"

#include <iostream>
#include <sstream>
//#include <ofstream.h>
#include <sys/stat.h>


ShapeFeat::ShapeFeat( InternalImageType::Pointer labelImage){
    this->labelImage = labelImage;

}

//int d = Dimension;

ShapeFeat::~ShapeFeat(){
    // Destructor. Not that necessary in our case.
}

void ShapeFeat::Run(){

    typedef InternalPixelType LabelType;
    typedef itk::ShapeLabelObject< LabelType, Dimension > LabelObjectType;
    typedef itk::LabelMap< LabelObjectType > LabelMapType;


      // Calculating the statistical parameters
      typedef itk::LabelImageToShapeLabelMapFilter< InternalImageType, LabelMapType > ShapeFilterType;
      ShapeFilterType::Pointer shapeFilter = ShapeFilterType::New();
      shapeFilter->SetInput( labelImage );
      shapeFilter->SetBackgroundValue( 0 ); // Label background Value
      shapeFilter->ComputeFeretDiameterOn();
      shapeFilter->ComputePerimeterOn();
      shapeFilter->Update();

      // Creating a shapeLabelMap to receive the filter output (which is N label maps).
      // Creating a shapeLabel Object to receive a specific labelMapObject (NthLabelObject).
      LabelMapType::Pointer shapeLabelMap = shapeFilter->GetOutput();
      LabelObjectType *shapeLabelObject = shapeLabelMap->GetNthLabelObject(0);

      InternalImageType::RegionType boundingRegion = shapeLabelObject->GetBoundingBox();
      InternalImageType::RegionType::SizeType size = boundingRegion.GetSize();
      double boundingVolume = size[0]*size[1]*size[2];

      //Bounding box volume over physical volue;
      double sizeRatio = boundingVolume / (shapeLabelObject->GetNumberOfPixels());

      /*
      std::cout<<"Number of Pixels: "<<shapeLabelObject->GetNumberOfPixels()<<std::endl;
      std::cout<<"Physical Size: "<<shapeLabelObject->GetPhysicalSize()<<std::endl;
      std::cout<<"Size Ratio: "<<sizeRatio<<std::endl;
      std::cout<<"Elongation: "<<shapeLabelObject->GetElongation()<<std::endl;
      std::cout<<"Feret Diameter: "<<shapeLabelObject->GetFeretDiameter()<<std::endl;
      std::cout<<"Equi. Sph. Perim.: "<<shapeLabelObject->GetEquivalentSphericalPerimeter()<<std::endl;
      std::cout<<"Equi. Sph. Radi.: "<<shapeLabelObject->GetEquivalentSphericalRadius()<<std::endl;
      std::cout<<"Perimeter: "<<shapeLabelObject->GetPerimeter()<<std::endl;
      std::cout<<"Roundness: "<<shapeLabelObject->GetRoundness()<<std::endl;
      //std::cout<<"Equi. Ellip. Diam.: "<<shapeLabelObject->GetEquivalentEllipsoidDiameter()<<std::endl;
      //std::cout<<"Flatness: "<<shapeLabelObject->GetFlatness()<<std::endl;
      //std::cout<<"Num. Pix. on Border: "<<shapeLabelObject->GetNumberOfPixelsOnBorder()<<std::endl;
      //std::cout<<"Perimeter on Border: "<<shapeLabelObject->GetPerimeterOnBorder()<<std::endl;
      //std::cout<<"Perim. on Bord. rat.: "<<shapeLabelObject->GetPerimeterOnBorderRatio()<<std::endl;
      std::cout<<"Centroid: "<<shapeLabelObject->GetCentroid()<<std::endl;
      std::cout<<"Principal Axes: "<<shapeLabelObject->GetPrincipalAxes()<<std::endl;
      std::cout<<"Principal Moments: "<<shapeLabelObject->GetPrincipalMoments()<<std::endl;
      */

      features[0] = shapeLabelObject->GetNumberOfPixels(); // In pixel numbers
      features[1] = shapeLabelObject->GetPhysicalSize(); // Numb. of pixels multiplied by the physical pixel size.
      features[2] = sizeRatio; // In pixel units
      features[3] = shapeLabelObject->GetElongation();
      features[4] = shapeLabelObject->GetFeretDiameter(); // In physical Units
      features[5] = shapeLabelObject->GetEquivalentSphericalPerimeter(); // In physical Units
      features[6] = shapeLabelObject->GetEquivalentSphericalRadius(); // In physical Units
      features[7] = shapeLabelObject->GetPerimeter(); // In physical Units
      features[8] = shapeLabelObject->GetRoundness();
}

double ShapeFeat::GetFeatures(int i){
    return features[i];
}

