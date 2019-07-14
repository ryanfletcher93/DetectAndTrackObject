#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <array>
#include <stdexcept>

//using namespace cv;

#define DEBUG 0
#define USE_CAMERA 0

#define CAM1_PARAM_FILE "cam1/calibResults"
#define CAM2_PARAM_FILE "cam2/calibResults"
#define DEFAULT_SAVE_FILE "stereo/StereoCameraProperties"

#define CAM1_IPADDR "192.168.1.7"
#define CAM2_IPADDR "192.168.1.9"

#define FILE1_TEMPLATE "cam1/background1WithObjects.png"
#define FILE2_TEMPLATE "cam2/background1WithObjects.png"
#define STEREO1_FILE_PATH "stereo/cam1/image*.png"
#define STEREO2_FILE_PATH "stereo/cam2/image*.png"
#define REPLACE_STR "\\*"

#define BOARD_WIDTH 9        // Number of squares the checkerboard is wide
#define BOARD_HEIGHT 7        // Number of squares the checkerboard is high
#define BOARD_SIZE cv::Size(BOARD_WIDTH, BOARD_HEIGHT)
#define BOARD_N (BOARD_WIDTH * BOARD_HEIGHT)
#define SQUARE_SIZE 20    // Number of mills of each checkerboard size

using std::string;
using std::vector;
using std::array;

using cv::Mat;

struct SingleCalibrationParams {
    Mat K, D;
} ;

struct StereoCalibrationParams {
    SingleCalibrationParams cam1Intrinsics, cam2Intrinsics;
    Mat R, E, F;
    cv::Vec3d T;
} ;

SingleCalibrationParams getCameraIntrinsicsFromFile(string);
array<Mat, 2> getImageFromCamera(int, string, string);
vector<array<Mat, 2>> getImagesFromFile(int, string, string, string);
StereoCalibrationParams stereoCalibrateWrapper(vector<array<Mat, 2>>, 
        SingleCalibrationParams, 
        SingleCalibrationParams);
void writeStereoParamsToFile(string, StereoCalibrationParams);
void saveImagesToFile(vector<Mat>, string, string);

/*
 *
 */
int main(int argc, char * argv[]) {
    vector<array<Mat, 2>> imagePairs;
    
    SingleCalibrationParams cam1CalibrationParams, cam2CalibrationParams;
    StereoCalibrationParams stereoCalibrationParams;

    string param1file, param2file;
    string outFile;

    // Set output file
    if (false) {
        outFile = argv[1];
    }
    else {
        outFile = DEFAULT_SAVE_FILE;
    }

    // Get stereo calibration images
    if (USE_CAMERA) {
        //imagePairs = getImageFromCamera(5, CAM1_IPADDR, CAM2_IPADDR);
    }
    else {
        imagePairs = getImagesFromFile(10, STEREO1_FILE_PATH, STEREO2_FILE_PATH, REPLACE_STR);
    }

    // Retrieve existing intrinsic camera properties from file using arguments
    cam1CalibrationParams = getCameraIntrinsicsFromFile(CAM1_PARAM_FILE);
    cam2CalibrationParams = getCameraIntrinsicsFromFile(CAM2_PARAM_FILE);

    // Calibrate the cameras
    stereoCalibrationParams = stereoCalibrateWrapper(imagePairs, 
            cam1CalibrationParams, cam2CalibrationParams);

    // Store the stereo camera properties in file
    writeStereoParamsToFile(outFile, stereoCalibrationParams);

    return 0;
}


/*
 *
 */
SingleCalibrationParams getCameraIntrinsicsFromFile(string file) {
    SingleCalibrationParams calibrationParams;
    cv::FileStorage fs(file, cv::FileStorage::READ);

    fs["K"] >> calibrationParams.K;
    fs["D"] >> calibrationParams.D;

    return calibrationParams;
}


/*
 *
 */
array<Mat, 2> getImageFromCamera(int numImages, 
        string ipAddr1, string ipAddr2) {

    array<Mat, 2> images;
    
    cv::VideoCapture video1("rtsp://" + ipAddr1 + ":554/onvif1");
    cv::VideoCapture video2("rtsp://" + ipAddr2 + ":554/onvif1");

    // Check for connection error
    if (!video1.isOpened() || !video2.isOpened()) {
        // TODO: Specify which camera is not connecting
        throw std::runtime_error("Unable to conect to a camera");
    }

    // Wait for user input then get the image at that point
    array<Mat, 2> tempImgArray;   

    Mat tempImg1, tempImg2;
    
    video1 >> tempImg1;
    video2 >> tempImg2;
    images[0] = tempImg1;
    images[1] = tempImg2;

    return images;
}

/*
 *
 */
void saveImagesToFile(vector<Mat> images, string filePattern, 
        string replaceStr) {

    std::regex r(replaceStr);
    string fileName;
    for (int i=0; i<images.size(); i++) {
        fileName = std::regex_replace (filePattern, r, std::to_string(i));

        cv::imwrite(fileName, images[i]);
    }
}


/* TODO: Implement getting images form files
 *
 */
vector<array<Mat, 2>> getImagesFromFile(int numImgs, string imgPath1, string imgPath2, 
        string replaceStr) {

    vector<array<Mat, 2>> imagePairs;

    std::regex r (replaceStr);
    for (int i=0; i<numImgs; i++) {
        string filePath1 = std::regex_replace(imgPath1, r, std::to_string(i));
        string filePath2 = std::regex_replace(imgPath2, r, std::to_string(i));

        Mat tempMat1 = cv::imread(filePath1, CV_LOAD_IMAGE_COLOR);
        Mat tempMat2 = cv::imread(filePath2, CV_LOAD_IMAGE_COLOR);

        array<Mat, 2> tempMatArray;
        tempMatArray[0] = tempMat1;
        tempMatArray[1] = tempMat2;

        imagePairs.push_back(tempMatArray);
    }

    return imagePairs;
}


/*
 *
 */
StereoCalibrationParams stereoCalibrateWrapper(vector<array<Mat, 2>> imagePairs, 
        SingleCalibrationParams cam1CalibrationParams, 
        SingleCalibrationParams cam2CalibrationParams) {
    
    vector<vector<cv::Point3f>> object_points;
    vector<vector<cv::Point2f>> imagePoints1, imagePoints2;
    vector<cv::Point2f> corners1, corners2;

    bool found1 = false, found2 = false;
    Mat gray1, gray2;
    for (const array<Mat, 2> &imgArr : imagePairs) {
        cvtColor(imgArr.at(0), gray1, CV_BGR2GRAY);
        cvtColor(imgArr.at(1), gray2, CV_BGR2GRAY);

        found1 = cv::findChessboardCorners(imgArr.at(0), BOARD_SIZE, corners1, 
            CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
        found2 = cv::findChessboardCorners(gray2, BOARD_SIZE, corners2, 
            CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);

        vector<cv::Point3f> obj;
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

    Mat R, E, F;
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
void writeStereoParamsToFile(string saveFile, StereoCalibrationParams stereoCalibrationParams) {
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
