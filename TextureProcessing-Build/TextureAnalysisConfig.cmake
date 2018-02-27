#
# This file makes available modules implemented in this extension to other extensions.
#

set(TextureAnalysis_SOURCE_DIR "/home/leonardo/Desktop/Modulos-Source/Modulos_to_Slicer/TextureProcessingExtension")

set(TextureAnalysis_LIBRARY_PATHS_LAUNCHER_BUILD "/home/leonardo/Desktop/Modulos-Source/Modulos_to_Slicer/TextureProcessingExtension/TextureProcessing-Build/lib/Slicer-4.9/cli-modules/${CMAKE_CFG_INTDIR};/home/leonardo/Desktop/Modulos-Source/Modulos_to_Slicer/TextureProcessingExtension/TextureProcessing-Build/lib/Slicer-4.9/qt-loadable-modules/${CMAKE_CFG_INTDIR}")
set(TextureAnalysis_PATHS_LAUNCHER_BUILD "/home/leonardo/Desktop/Modulos-Source/Modulos_to_Slicer/TextureProcessingExtension/TextureProcessing-Build/lib/Slicer-4.9/cli-modules/${CMAKE_CFG_INTDIR};/home/leonardo/Desktop/Modulos-Source/Modulos_to_Slicer/TextureProcessingExtension/TextureProcessing-Build/bin/${CMAKE_CFG_INTDIR}")
set(TextureAnalysis_ENVVARS_LAUNCHER_BUILD "")
set(TextureAnalysis_PYTHONPATH_LAUNCHER_BUILD "/home/leonardo/Desktop/Modulos-Source/Modulos_to_Slicer/TextureProcessingExtension/TextureProcessing-Build/lib/Slicer-4.9/qt-scripted-modules;/home/leonardo/Desktop/Modulos-Source/Modulos_to_Slicer/TextureProcessingExtension/TextureProcessing-Build/lib/Slicer-4.9/qt-loadable-modules/${CMAKE_CFG_INTDIR};/home/leonardo/Desktop/Modulos-Source/Modulos_to_Slicer/TextureProcessingExtension/TextureProcessing-Build/lib/Slicer-4.9/qt-loadable-modules/Python")



# --------------------------------------------------------------------------
# Include directories

# Extension include directories for module


# Extension include directories for module logic


# Extension include directories for module mrml


# Extension include directories for module Widget


# Extension VTK wrap hierarchy files


# Extension Module logic include file directories.
set(TextureAnalysis_ModuleLogic_INCLUDE_DIRS ""
  CACHE INTERNAL "Extension Module logic includes" FORCE)

# Extension Module MRML include file directories.
set(TextureAnalysis_ModuleMRML_INCLUDE_DIRS ""
  CACHE INTERNAL "Extension Module MRML includes" FORCE)

# Extension Module Widgets include file directories.
set(TextureAnalysis_ModuleWidgets_INCLUDE_DIRS ""
  CACHE INTERNAL "Extension Module widgets includes" FORCE)

# --------------------------------------------------------------------------
# Target definitions

if(EXISTS "/home/leonardo/Desktop/Modulos-Source/Modulos_to_Slicer/TextureProcessingExtension/TextureProcessing-Build/./TextureAnalysisTargets.cmake")
  include("/home/leonardo/Desktop/Modulos-Source/Modulos_to_Slicer/TextureProcessingExtension/TextureProcessing-Build/./TextureAnalysisTargets.cmake")
endif()
