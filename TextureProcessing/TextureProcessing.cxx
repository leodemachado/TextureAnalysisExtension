/*
 * Main class of the Digital Image Texture Extraction Extension
 * The present CLI module receives a Grey-Level Image and a Label Map imgae that defines
 * a region of interest (ROI). The present CLI module preprocess the image converting it (the gray
 * level image) into a known data type. Then it applies the ItkBiasFieldCorrection filter
 * enphasizing the ROI. Finishing the preprocessing routine, it carries out a normalization
 * step that rescales the image into the histoSize (Global.h file) pixel intensity range
 * (by default, 1 - 128) and removes outlier pixels (details are in Normalize.cxx) from ROI.
 * After preprocessing, the pipeline runs extracting 48 textural features based on 5 different texture analysis
 * methods. Finally it writes all those features in a .csv file.
*/

#include "itkPluginUtilities.h"
#include "TextureProcessingCLP.h"

#include "Global.h"

#include "itkImageFileWriter.h"
#include <itkImageDuplicator.h>
#include <itkCastImageFilter.h>
#include "itkN4BiasFieldCorrectionImageFilter.h"
#include "itkBinaryDilateImageFilter.h"
#include "itkBinaryBallStructuringElement.h"

#include "Plot/HistogramPlot.h"
#include "Percents/PercentileCalc.h"
#include "Features/HistoFeat.h"
#include "CoocurrenceFeat/CoocurrenceFeat.h"
#include "ShapeFeatures/ShapeFeat.h"
#include "RunLengthFeat/RunLengthFeat.h"
#include "Normalization/Normalize.h"
#include "Gradient/GradientFeat.h"

#include <sys/stat.h>

// Use an anonymous namespace to keep class types and function names
// from colliding when module is used as shared object module.  Every
// thing should be in an anonymous namespace except for the module
// entry point, e.g. main()

namespace
{

template <class T>
int DoIt( int argc, char * argv[], T )
{
    PARSE_ARGS;

    if( argc < 3 )
    {
        std::cerr << "Missing command line arguments" << std::endl;
        std::cerr << "Usage :  Image  inputImageFileName " << std::endl;
        return EXIT_FAILURE;
    }

    typedef    T InputPixelType;
    typedef itk::Image<InputPixelType,  Dimension> InputImageType;
    typedef itk::ImageFileReader<InputImageType>  ReaderType;

    // ###############
    std::cout << "Etapa 1 - Reading" <<std::endl;

    // Instantiating and reading inputs
    typename ReaderType::Pointer imageReader = ReaderType::New();
    imageReader->SetFileName( inputVolume.c_str() );
    imageReader->Update();

    typename ReaderType::Pointer labelReader = ReaderType::New();
    labelReader->SetFileName( inputLabel.c_str() );
    labelReader->Update();

    // ###############
    // catching filename (with extension) to use in saving file routine

    std::string fullname = inputVolume.c_str();
    std::string fileNameWithExtension = fullname.substr(fullname.find_last_of("\\/")+1);
    // Removing extension
    std::string fileName = fileNameWithExtension.substr(0, fileNameWithExtension.find_last_of("."));
    // ###############

    // ###############
    std::cout << "Etapa 2 - Pre-processing" <<std::endl;

    // Cast short Filter over feature image
    typedef itk::CastImageFilter<InputImageType, InternalImageType> CastFilterType;
    typename CastFilterType::Pointer labelCastFilter = CastFilterType::New();
    labelCastFilter->SetInput(labelReader->GetOutput());
    labelCastFilter->Update();

    // Defining Eroding parameter (Structuring Element)
    int radius = 8;
    typedef itk::BinaryBallStructuringElement<InternalPixelType,Dimension> StructuringElementType;
    StructuringElementType structuringElement;
    structuringElement.SetRadius(radius);
    structuringElement.CreateStructuringElement();

    // Dilating mask for bIAS correction
    typedef itk::BinaryDilateImageFilter <InternalImageType, InternalImageType, StructuringElementType>
            BinaryDilateImageFilterType;
    typename BinaryDilateImageFilterType::Pointer dilateFilter = BinaryDilateImageFilterType::New();
    dilateFilter->SetInput(labelCastFilter->GetOutput());
    dilateFilter->SetKernel(structuringElement);

    // N4 Bias Field Correction
    typedef itk::N4BiasFieldCorrectionImageFilter<InputImageType,InternalImageType> CorrectionFilterType;
    typename CorrectionFilterType::Pointer correctionFilter = CorrectionFilterType::New();
    correctionFilter->SetInput(imageReader->GetOutput());
    correctionFilter->SetMaskImage(dilateFilter->GetOutput());

    try
    {
        correctionFilter->Update();
    }
    catch( itk::ExceptionObject & excp )
    {
        std::cerr << "Problem correcting image image file : " << argv[1] << std::endl;
        std::cerr << excp << std::endl;
        return EXIT_FAILURE;
    }

    // Cast short Filter over feature image
    typename CastFilterType::Pointer imageCastFilter = CastFilterType::New();
    imageCastFilter->SetInput(correctionFilter->GetOutput());
    imageCastFilter->Update();

    typename InternalImageType::Pointer featureImage;
    typename InternalImageType::Pointer labelImage;

    //* End of pre processing.
    // ###############

    // ###############
    std::cout << "Etapa 3 - Normalization" <<std::endl;

    if(doNormalization){
        Normalize* normalize = new Normalize(imageCastFilter->GetOutput(),labelCastFilter->GetOutput());
        normalize->Run();
        // Grayscale Image
        featureImage = normalize->GetNormalized(0);
        // Label Image
        labelImage = normalize->GetNormalized(1);

        std::cout << "Etapa ** - Normalized" <<std::endl;

    } else{
        // If it's chosen not to normalize pixel intensities, it's given the original image.
        featureImage = imageCastFilter->GetOutput();
        labelImage = labelCastFilter->GetOutput();

        std::cout << "Etapa **** - NOT Normalized" <<std::endl;
    }

    std::cout << "Etapa 4 - Saving Path Configuration" <<std::endl;
    // ###############

    struct stat sb;
    stringstream temp;

    // Checks if spath /TextureProcessing alredy exists. If not, creates it.
    spath = spath+"/TextureProcessing";       // Updates My spath Variable!
    temp.str(std::string());

    // if spath/Textureprocessing doesn't exist, creates it!
    if (!stat(spath.c_str(), &sb) == 0 || !S_ISDIR(sb.st_mode)){
        temp<<"mkdir "<<spath;
        system(temp.str().c_str());
    }

    // Combining the strings into one unique string variable to create the fileName.csv file.
    std::string FileHistoStat = spath + "/" + fileName + ".csv";
    // std::string FileHistoStat = fileName + ".csv";
    const char * fHS = FileHistoStat.c_str();
    std::ofstream features( fHS );

    // Combining the strings into one unique string variable to create the Histogram.txt file.
    std::string FileHisto = spath + "/Histogram.txt";
    const char * fH = FileHisto.c_str();
    std::ofstream histo( fH );

    // Combining the strings into one unique string variable to create the Percents.txt file.
    std::string FilePerc = spath + "/Percents.txt";
    const char * fP = FilePerc.c_str();
    std::ofstream percents( fP );

    // End of path configuration
    // ###############


    // ###############
    std::cout << "Etapa 5 - Calling Processing Functions" <<std::endl;

    // Calling the plotting function
    HistogramPlot* histogram = new HistogramPlot( featureImage, labelImage);
    histogram->Run();

    // Receiving the frequecy container vector;
    int freq[histoSize];
    for (int i = 0; i<histoSize; i++){
        freq[i] = histogram->GetHistogram(i);
    }

    if(doHistoPlot){
        // Histogram

        histo << "#Histogram size " << histoSize << std::endl;
        histo << "#bin   frequency" << std::endl;
        for ( int bin=0; bin < histoSize; ++bin ) {
            histo << bin << "    " << histogram->GetHistogram(bin)<< std::endl;
        }
        histo.close();
    }

    if(doPercentiles){
        PercentileCalc* percentiles = new PercentileCalc( freq, spath);
        percentiles->Run();

        // Percents
        percents << "#Percents " << histoSize << std::endl;
        percents << "#th   percentile bin" << std::endl;
        for ( int p=1; p < 101; ++p ) {
            percents << p << "th    " << percentiles->GetPercentiles(p)<< std::endl;
        }
        percents.close();

        // Specific Percentis Features and Grey level Entropy
        features<<"Percent-5th "<<percentiles->GetPercentiles(5)<< std::endl;
        features<<"Percent-10th "<<percentiles->GetPercentiles(10)<< std::endl;
        features<<"Percent-25th "<<percentiles->GetPercentiles(25)<< std::endl;
        features<<"Percent-75th "<<percentiles->GetPercentiles(75)<< std::endl;
        features<<"Percent-90th "<<percentiles->GetPercentiles(90)<< std::endl;
        features<<"Quartile "<<abs(percentiles->GetPercentiles(75) - percentiles->GetPercentiles(25))<< std::endl;
        features<<"GL-Entropy "<<percentiles->GetPercentiles(0)<< std::endl;

        std::cout << "**Percentile - ok" << std::endl;
    }

    if(doHistogramFeat){
        HistoFeat* histoFeatures = new HistoFeat(featureImage, labelImage);
        histoFeatures->Run();

        // Histo Features
        features<<"Mean "<<histoFeatures->GetFeatures(0)<< std::endl;
        features<<"Std-Deviation "<<histoFeatures->GetFeatures(1)<< std::endl;
        features<<"Median "<<histoFeatures->GetFeatures(2)<< std::endl;
        features<<"Skewness "<<histoFeatures->GetFeatures(3)<< std::endl;
        features<<"Kurtosis "<<histoFeatures->GetFeatures(4)<< std::endl;
        features<<"Variance "<<histoFeatures->GetFeatures(6)<< std::endl;
        features<<"Histogram-Elongation "<<histoFeatures->GetFeatures(7)<< std::endl;
        features<<"Flatness "<<histoFeatures->GetFeatures(8)<< std::endl;
        features<<"Minimum "<<histoFeatures->GetFeatures(9)<< std::endl;
        features<<"Maximum "<<histoFeatures->GetFeatures(10)<< std::endl;

        std::cout << "**Histogram Features - ok" << std::endl;
    }

    if(doCoocurrenceFeat){
        CoocurrenceFeat* coocurrenceFeat = new CoocurrenceFeat(featureImage, labelImage);
        coocurrenceFeat->Run();

        // Coocurrence Features
        features<<"Entropy "<<coocurrenceFeat->GetFeatures(0)<< std::endl;
        features<<"Energy "<<coocurrenceFeat->GetFeatures(1)<< std::endl;
        features<<"Correlation "<<coocurrenceFeat->GetFeatures(2)<< std::endl;
        features<<"Haralick-Correlation "<<coocurrenceFeat->GetFeatures(3)<< std::endl;
        features<<"Cluster-Proeminence "<<coocurrenceFeat->GetFeatures(4)<< std::endl;
        features<<"Cluster-Shade "<<coocurrenceFeat->GetFeatures(5)<< std::endl;
        features<<"Inertia " <<coocurrenceFeat->GetFeatures(6)<< std::endl;
        features<<"Inverse-Difference-Moment "<<coocurrenceFeat->GetFeatures(7)<< std::endl;

        std::cout << "**Coocurrence Features - ok" << std::endl;
    }

    if(doRunFeat){
        RunLengthFeat* runLengthFeat = new RunLengthFeat(featureImage, labelImage);
        runLengthFeat->Run();

        // RunLength Features
        features<<"SRE "<<runLengthFeat->GetFeatures(0)<< std::endl;
        features<<"LRE "<<runLengthFeat->GetFeatures(1)<< std::endl;
        features<<"GLN "<<runLengthFeat->GetFeatures(2)<< std::endl;
        features<<"RLN "<<runLengthFeat->GetFeatures(3)<< std::endl;
        features<<"LGRE "<<runLengthFeat->GetFeatures(4)<< std::endl;
        features<<"HGRE "<<runLengthFeat->GetFeatures(5)<< std::endl;
        features<<"SRLGE "<<runLengthFeat->GetFeatures(6)<< std::endl;
        features<<"SRHGE "<<runLengthFeat->GetFeatures(7)<< std::endl;
        features<<"LRLGE "<<runLengthFeat->GetFeatures(8)<< std::endl;
        features<<"LRHGE "<<runLengthFeat->GetFeatures(9)<< std::endl;
        features<<"RP "<<runLengthFeat->GetFeatures(10)<< std::endl;

        std::cout << "**RunLength Features - ok" << std::endl;
    }

    if(doShapeFeat){
        ShapeFeat* shapeFeatures = new ShapeFeat(labelImage);
        shapeFeatures->Run();

        // Shape Features
        features<<"Volume-In-Physical-Units "<<shapeFeatures->GetFeatures(1)<< std::endl;
        features<<"Size-Ratio "<<shapeFeatures->GetFeatures(2)<< std::endl;
        features<<"Elongation "<<shapeFeatures->GetFeatures(3)<< std::endl;
        features<<"Feret-Diameter "<<shapeFeatures->GetFeatures(4)<< std::endl;
        features<<"Equivalent-Spherical-Perimeter "<<shapeFeatures->GetFeatures(5)<< std::endl;
        features<<"Equivalent-Spherical-Radius "<<shapeFeatures->GetFeatures(6)<< std::endl;
        features<<"Perimeter "<<shapeFeatures->GetFeatures(7)<< std::endl;
        features<<"Roundness "<<shapeFeatures->GetFeatures(8)<< std::endl;

        std::cout << "**Shape Features - ok" << std::endl;
    }

    if(doGradient){
        GradientFeat* gradientFeatures = new GradientFeat(featureImage, labelImage);
        gradientFeatures->Run();

        //Gradient Features
        features<<"Gradient-Mean "<<gradientFeatures->GetFeatures(0)<< std::endl;
        features<<"Gradient-Std "<<gradientFeatures->GetFeatures(1)<< std::endl;
        features<<"Gradient-Variance "<<gradientFeatures->GetFeatures(2)<< std::endl;
        features<<"Gradient-Maximum "<<gradientFeatures->GetFeatures(3)<< std::endl;

        std::cout << "**Gradient Features - ok" <<std::endl;
    }

    features.close();

    std::cout << "End execution!" <<std::endl;
    // ###############

    return EXIT_SUCCESS;
}

} // end of anonymous namespace

int main( int argc, char * argv[] )
{
    PARSE_ARGS;

    itk::ImageIOBase::IOPixelType     pixelType;
    itk::ImageIOBase::IOComponentType componentType;

    try
    {
        itk::GetImageType(inputVolume, pixelType, componentType);

        // This filter handles all types on input, but only produces
        // signed types
        switch( componentType )
        {
        case itk::ImageIOBase::UCHAR:
            return DoIt( argc, argv, static_cast<unsigned char>(0) );
            break;
        case itk::ImageIOBase::CHAR:
            return DoIt( argc, argv, static_cast<char>(0) );
            break;
        case itk::ImageIOBase::USHORT:
            return DoIt( argc, argv, static_cast<unsigned short>(0) );
            break;
        case itk::ImageIOBase::SHORT:
            return DoIt( argc, argv, static_cast<short>(0) );
            break;
        case itk::ImageIOBase::UINT:
            return DoIt( argc, argv, static_cast<unsigned int>(0) );
            break;
        case itk::ImageIOBase::INT:
            return DoIt( argc, argv, static_cast<int>(0) );
            break;
        case itk::ImageIOBase::ULONG:
            return DoIt( argc, argv, static_cast<unsigned long>(0) );
            break;
        case itk::ImageIOBase::LONG:
            return DoIt( argc, argv, static_cast<long>(0) );
            break;
        case itk::ImageIOBase::FLOAT:
            return DoIt( argc, argv, static_cast<float>(0) );
            break;
        case itk::ImageIOBase::DOUBLE:
            return DoIt( argc, argv, static_cast<double>(0) );
            break;
        case itk::ImageIOBase::UNKNOWNCOMPONENTTYPE:
        default:
            std::cout << "unknown component type" << std::endl;
            break;
        }
    }

    catch( itk::ExceptionObject & excep )
    {
        std::cerr << argv[0] << ": exception caught !" << std::endl;
        std::cerr << excep << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
