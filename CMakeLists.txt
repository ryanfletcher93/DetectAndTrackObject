cmake_minimum_required(VERSION 2.6.0)
project(object_tracker)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

include_directories(include)

file(GLOB SOURCES "src/main/*.cpp" "src/camera/*.cpp" "src/detection/*.cpp" "src/stereoCamera/*.cpp" "src/tracking/*.cpp" "src/actionEndEffector/*.cpp" "src/motor/*.cpp")

add_executable(objectTracker ${SOURCES})

link_directories(/usr/local/include/opencv4/opencv2)
target_link_libraries(objectTracker opencv_core opencv_imgproc opencv_highgui opencv_calib3d opencv_features2d)

