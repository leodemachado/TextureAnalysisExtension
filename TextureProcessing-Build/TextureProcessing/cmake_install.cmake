# Install script for directory: /home/leonardo/Desktop/Modulos-Source/Modulos_to_Slicer/TextureProcessingExtension/TextureProcessing

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "RuntimeLibraries" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./lib/Slicer-4.9/cli-modules/TextureProcessing" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./lib/Slicer-4.9/cli-modules/TextureProcessing")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./lib/Slicer-4.9/cli-modules/TextureProcessing"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/./lib/Slicer-4.9/cli-modules" TYPE EXECUTABLE FILES "/home/leonardo/Desktop/Modulos-Source/Modulos_to_Slicer/TextureProcessingExtension/TextureProcessing-Build/lib/Slicer-4.9/cli-modules/TextureProcessing")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./lib/Slicer-4.9/cli-modules/TextureProcessing" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./lib/Slicer-4.9/cli-modules/TextureProcessing")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./lib/Slicer-4.9/cli-modules/TextureProcessing"
         OLD_RPATH "/usr/local/Slicer/teem-build/bin:/usr/local/Slicer/qRestAPI-build:/home/leonardo/Desktop/Modulos-Source/Modulos_to_Slicer/TextureProcessingExtension/TextureProcessing-Build/lib/Slicer-4.9/cli-modules:/usr/local/Slicer/Slicer-build/bin:/usr/local/Slicer/JsonCpp-build/src/lib_json/.:/usr/local/Slicer/ITKv4-build/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./lib/Slicer-4.9/cli-modules/TextureProcessing")
    endif()
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "RuntimeLibraries" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./lib/Slicer-4.9/cli-modules/libTextureProcessingLib.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./lib/Slicer-4.9/cli-modules/libTextureProcessingLib.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./lib/Slicer-4.9/cli-modules/libTextureProcessingLib.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/./lib/Slicer-4.9/cli-modules" TYPE SHARED_LIBRARY FILES "/home/leonardo/Desktop/Modulos-Source/Modulos_to_Slicer/TextureProcessingExtension/TextureProcessing-Build/lib/Slicer-4.9/cli-modules/libTextureProcessingLib.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./lib/Slicer-4.9/cli-modules/libTextureProcessingLib.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./lib/Slicer-4.9/cli-modules/libTextureProcessingLib.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./lib/Slicer-4.9/cli-modules/libTextureProcessingLib.so"
         OLD_RPATH "/usr/local/Slicer/teem-build/bin:/usr/local/Slicer/qRestAPI-build:/usr/local/Slicer/ITKv4-build/lib:/usr/local/Slicer/JsonCpp-build/src/lib_json/.:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./lib/Slicer-4.9/cli-modules/libTextureProcessingLib.so")
    endif()
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "RuntimeLibraries" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/./lib/Slicer-4.9/cli-modules" TYPE FILE FILES "/home/leonardo/Desktop/Modulos-Source/Modulos_to_Slicer/TextureProcessingExtension/TextureProcessing/TextureProcessing.xml")
endif()

