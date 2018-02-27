# TextureAnalysisExtension
The present project is the implementation of a Digital Image Texture Extraction Extension.
This CLI module receives a Grey-Level Image and a Label Map imgae that defines
a region of interest (ROI). The present CLI module preprocess the image converting it (the gray
level image) into a known data type. Then it applies the ItkBiasFieldCorrection filter
enphasizing the ROI. Finishing the preprocessing routine, it carries out a normalization
step that rescales the image into the histoSize (Global.h file) pixel intensity range
(by default, 1 - 128) and removes outlier pixels (details are in Normalize.cxx) from ROI.
After preprocessing, the pipeline runs extracting 48 textural features based on 5 different texture analysis
methods. Finally it writes all those features in a '.csv' file.
