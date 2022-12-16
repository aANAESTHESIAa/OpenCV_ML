cmake_minimum_required(VERSION 3.19)
project(CV_LR2_ConsoleImageEditor)

set(CMAKE_CXX_STANDARD 14)

add_executable(CV_LR2_ConsoleImageEditor main.cpp functions.h functions.cpp)

# Where to find CMake modules and OpenCV
set(OpenCV_DIR "C:\\opencv\\mingw-build\\install")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

# add libs you need
set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)

# linking
target_link_libraries(CV_LR2_ConsoleImageEditor ${OpenCV_LIBS})