cmake_minimum_required (VERSION 2.6) 
project (CAUGens)

include_directories(${SC_PATH}/include/plugin_interface) 
include_directories(${SC_PATH}/include/common) 
#include_directories(${SC_PATH}/server/plugins) #optional, for FFT_UGens.h
include_directories(${SC_PATH}/external_libraries/libsndfile/) 

set(CMAKE_SHARED_MODULE_PREFIX "")
if(APPLE OR WIN32)
set(CMAKE_SHARED_MODULE_SUFFIX ".scx")
endif()

add_library(CAUGens MODULE CAUGens.cpp) 

#to compile run:
#cmake -DSC_PATH=/Users/yota/Deployment/supercollider .
#then
#make