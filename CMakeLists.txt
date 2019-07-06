cmake_minimum_required(VERSION 2.6.0)
project(object_tracker)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

include_directories(include)

file(GLOB CORE_SOURCES "src/camera/*.cpp" "src/detection/*.cpp" "src/stereoCamera/*.cpp" "src/tracking/*.cpp" "src/actionEndEffector/*.cpp" "src/motor/*.cpp" "src/calibration/*.cpp")

file(GLOB OBJECT_TRACKER_SOURCES "src/main/trackerRunner.cpp")
file(GLOB DETECTION_TEST_SOURCES "src/main/detectionTest.cpp")

add_executable(objectTracker ${CORE_SOURCES} ${OBJECT_TRACKER_SOURCES})
add_executable(detectionTest ${CORE_SOURCES} ${DETECTION_TEST_SOURCES})

link_directories(/usr/local/include/opencv4/opencv2)
target_link_libraries(objectTracker opencv_core opencv_imgproc opencv_highgui opencv_calib3d opencv_features2d)

target_link_libraries(detectionTest opencv_core opencv_imgproc opencv_highgui opencv_calib3d opencv_features2d)
