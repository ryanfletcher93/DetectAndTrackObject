#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <array>
#include <stdexcept>

//using namespace cv;

#define DEBUG 0
#define USE_CAMERA 1

#define CAM1_PARAM_FILE "cam1/calibResults"
#define CAM2_PARAM_FILE "cam2/calibResults"
#define DEFAULT_SAVE_FILE "stereo/StereoCameraProperties"

#define CAM1_IPADDR "192.168.1.7"
#define CAM2_IPADDR "192.168.1.9"

#define FILE1_TEMPLATE "cam1/background1WithObjects.png"
#define FILE2_TEMPLATE "cam2/background1WithObjects.png"
#define REPLACE_STR "\\*"

#define BOARD_WIDTH 9        // Number of squares the checkerboard is wide
#define BOARD_HEIGHT 7        // Number of squares the checkerboard is high
#define BOARD_SIZE cv::Size(BOARD_WIDTH, BOARD_HEIGHT)
#define BOARD_N (BOARD_WIDTH * BOARD_HEIGHT)
#define SQUARE_SIZE 20    // Number of mills of each checkerboard size

struct SingleCalibrationParams {
    cv::Mat K, D;
} ;

struct StereoCalibrationParams {
    SingleCalibrationParams cam1Intrinsics, cam2Intrinsics;
    cv::Mat R, E, F;
    cv::Vec3d T;
} ;

SingleCalibrationParams getCameraIntrinsicsFromFile(std::string);
std::array<cv::Mat, 2> getImageFromCamera(int, std::string, std::string);
std::vector<std::array<cv::Mat, 2>> getImagesFromFile(std::string);
StereoCalibrationParams stereoCalibrateWrapper(std::vector<std::array<cv::Mat, 2>>, 
        SingleCalibrationParams, 
        SingleCalibrationParams);
void writeStereoParamsToFile(std::string, StereoCalibrationParams);
void saveImagesToFile(std::vector<cv::Mat>, std::string, std::string);

/*
 *
 */
int main(int argc, char * argv[]) {
    std::array<cv::Mat, 2> imagePairs;
    
    SingleCalibrationParams cam1CalibrationParams, cam2CalibrationParams;
    StereoCalibrationParams stereoCalibrationParams;

    std::string param1file, param2file;
    std::string outFile;

    // Set output file
    if (false) {
        outFile = argv[1];
    }
    else {
        outFile = DEFAULT_SAVE_FILE;
    }

    // Get stereo calibration images
    if (USE_CAMERA) {
        imagePairs = getImageFromCamera(5, CAM1_IPADDR, CAM2_IPADDR);
    }
    else {
        //imagePairs = getImagesFromFile();
    }

    imwrite(FILE1_TEMPLATE, imagePairs[0]);
    imwrite(FILE2_TEMPLATE, imagePairs[1]);

    // Retrieve existing intrinsic camera properties from file using arguments
    //cam1CalibrationParams = getCameraIntrinsicsFromFile(CAM1_PARAM_FILE);
    //cam2CalibrationParams = getCameraIntrinsicsFromFile(CAM2_PARAM_FILE);

    // Calibrate the cameras
    //stereoCalibrationParams = stereoCalibrateWrapper(imagePairs, 
    //        cam1CalibrationParams, cam2CalibrationParams);

    // Store the stereo camera properties in file
    //writeStereoParamsToFile(outFile, stereoCalibrationParams);

    return 0;
}


/*
 *
 */
SingleCalibrationParams getCameraIntrinsicsFromFile(std::string file) {
    SingleCalibrationParams calibrationParams;
    cv::FileStorage fs(file, cv::FileStorage::READ);

    fs["K"] >> calibrationParams.K;
    fs["D"] >> calibrationParams.D;

    return calibrationParams;
}


/*
 *
 */
std::array<cv::Mat, 2> getImageFromCamera(int numImages, 
        std::string ipAddr1, std::string ipAddr2) {

    std::array<cv::Mat, 2> images;
    
    cv::VideoCapture video1("rtsp://" + ipAddr1 + ":554/onvif1");
    cv::VideoCapture video2("rtsp://" + ipAddr2 + ":554/onvif1");

    // Check for connection error
    if (!video1.isOpened() || !video2.isOpened()) {
        // TODO: Specify which camera is not connecting
        throw std::runtime_error("Unable to conect to a camera");
    }

    // Wait for user input then get the image at that point
    std::array<cv::Mat, 2> tempImgArray;   

    cv::Mat tempImg1, tempImg2;
    
    video1 >> tempImg1;
    video2 >> tempImg2;
    images[0] = tempImg1;
    images[1] = tempImg2;

    return images;
}

/*
 *
 */
void saveImagesToFile(std::vector<cv::Mat> images, std::string filePattern, 
        std::string replaceStr) {

    std::regex r(replaceStr);
    std::string fileName;
    for (int i=0; i<images.size(); i++) {
        fileName = std::regex_replace (filePattern, r, std::to_string(i));

        cv::imwrite(fileName, images[i]);
    }
}


/* TODO: Implement getting images form files
 *
 */
std::vector<std::array<cv::Mat, 2>> getImagesFromFile(std::string fileName) {
    std::vector<std::array<cv::Mat, 2>> imagePairs;

    return imagePairs;
}


/*
 *
 */
StereoCalibrationParams stereoCalibrateWrapper(std::vector<std::array<cv::Mat, 2>> imagePairs, 
        SingleCalibrationParams cam1CalibrationParams, 
        SingleCalibrationParams cam2CalibrationParams) {
    
    std::vector<std::vector<cv::Point3f>> object_points;
    std::vector<std::vector<cv::Point2f>> imagePoints1, imagePoints2;
    std::vector<cv::Point2f> corners1, corners2;

    bool found1 = false, found2 = false;
    cv::Mat gray1, gray2;
    for (const std::array<cv::Mat, 2> &imgArr : imagePairs) {
        cvtColor(imgArr.at(0), gray1, CV_BGR2GRAY);
        cvtColor(imgArr.at(1), gray2, CV_BGR2GRAY);

        found1 = cv::findChessboardCorners(imgArr.at(0), BOARD_SIZE, corners1, 
            CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
        found2 = cv::findChessboardCorners(gray2, BOARD_SIZE, corners2, 
            CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);

        std::vector<cv::Point3f> obj;
        for (int i=0; i<BOARD_HEIGHT; i++) {
            for (int j=0; j<BOARD_WIDTH; j++) {
                obj.push_back(cv::Point3f((float) j*SQUARE_SIZE, (float) i*SQUARE_SIZE, 0));
            }
        }

        if (found1 && found2) {
            std::cout << "Found both corners" << std::endl;
            object_points.push_back(obj);
            imagePoints1.push_back(corners1);
            imagePoints2.push_back(corners2);
        }
    }

    cv::Mat R, E, F;
    cv::Vec3d T;

    cv::stereoCalibrate(
            object_points, 
            imagePoints1, 
            imagePoints2, 
            cam1CalibrationParams.K, 
            cam1CalibrationParams.D, 
            cam2CalibrationParams.K, 
            cam2CalibrationParams.D, 
            gray1.size(), 
            R, 
            T, 
            E, 
            F
    );

    StereoCalibrationParams stereoCalibrationParams;
    stereoCalibrationParams.cam1Intrinsics = cam1CalibrationParams;
    stereoCalibrationParams.cam2Intrinsics = cam2CalibrationParams;
    stereoCalibrationParams.R = R;
    stereoCalibrationParams.T = T;
    stereoCalibrationParams.E = E;
    stereoCalibrationParams.F = F;

    return stereoCalibrationParams;
}


/*
 *
 */
void writeStereoParamsToFile(std::string saveFile, StereoCalibrationParams stereoCalibrationParams) {
    cv::FileStorage fs(saveFile, cv::FileStorage::WRITE);

    fs << "K1" << stereoCalibrationParams.cam1Intrinsics.K;
    fs << "D1" << stereoCalibrationParams.cam1Intrinsics.D;
    fs << "K2" << stereoCalibrationParams.cam2Intrinsics.K;
    fs << "D2" << stereoCalibrationParams.cam2Intrinsics.D;

    fs << "R" << stereoCalibrationParams.R;
    fs << "T" << stereoCalibrationParams.T;
    fs << "E" << stereoCalibrationParams.E;
    fs << "F" << stereoCalibrationParams.F;
}