cmake_minimum_required(VERSION 3.20)

project(SNOMFft LANGUAGES CXX)

set(SNOMFitting_LIBRARIES SNOMFft)
set(CMAKE_BUILD_TYPE Release)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ENABLE_TESTS OFF)
set(ENABLE_DFT ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2")
#add_subdirectory(../../3rdparty/kfr)

find_library(KFR NAMES kfr_dft HINTS ../../3rdparty/kfr/release/libs REQUIRED)

list(APPEND CMAKE_PREFIX_PATH "G:/msys64/mingw64/lib/cmake/Qt6/")

#set(CMAKE_PREFIX_PATH=/mingw64/lib/cmake/Qt6/)
include_directories(../../3rdparty/kfr/include) #release
include_directories(../..) #release

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Core REQUIRED)

add_library(SNOM-Fft SHARED
  snom_fft.cpp
  ../../base/datamanager/snom_dataset_2.cpp
  ../../base/helper/snom_helper.cpp
)

target_compile_definitions(SNOM-Fft PRIVATE SNOM_FFT_LIBRARY)

target_link_libraries(SNOM-Fft PRIVATE fmt Qt6::Core ${KFR})
                       


