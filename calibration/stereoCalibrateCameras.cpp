#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <stdexcept>

using namespace cv;

#define DEBUG 0
#define USE_CAMERA 1

#define BOARD_WIDTH 4        // Number of squares the checkerboard is wide
#define BOARD_HEIGHT 4        // Number of squares the checkerboard is high
#define BOARD_SIZE Size(BOARD_WIDTH, BOARD_HEIGHT)
#define BOARD_N (BOARD_WIDTH * BOARD_HEIGHT)
#define SQUARE_SIZE 24.23    // Number of mills of each checkerboard size

struct SingleCalibrationParams {
    Mat K, D;
} ;

struct StereoCalibrationParams {
    SingleCalibrationParams cam1Intrinsics, cam2Intrinsics;
    Mat R, E, F;
    Vec3d T;
} ;

SingleCalibrationParams getCameraIntrinsicsFromFile(std::string);
std::vector<std::array<Mat, 2>> getImagesFromCamera(int, string);
std::vector<std::array<Mat, 2>> getImagesFromFile(string);
StereoCalibrationParams stereoCalibrateWrapper(std::vector<std::array<Mat, 2>>, 
        SingleCalibrationParams, 
        SingleCalibrationParams);
void writeStereoParamsToFile(std::string, StereoCalibrationParams);


/*
 *
 */
int main(int argc, char * argv[]) {
    std::vector<std::array<Mat, 2>> imagePairs;
    
    SingleCalibrationParams cam1CalibrationParams, cam2CalibrationParams;
    StereoCalibrationParams stereoCalibrationParams;

    std::string param1file, param2file;
    std::string outFile;

    if (DEBUG) {
        param1file = "";
        param2file = "";
    }
    else {
        param1file = argv[1];
        param2file = argv[2];
    }

    // Get stereo calibration images
    if (USE_CAMERA) {
        //imagePairs = getImagesFromCamera(1, "192.168.1.2", "192.168.1.3");
    }
    else {
        //imagePairs = getImagesFromFile();
    }

    // Retrieve existing intrinsic camera properties from file using arguments
    cam1CalibrationParams = getCameraIntrinsicsFromFile(param1file);
    cam2CalibrationParams = getCameraIntrinsicsFromFile(param2file);

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
SingleCalibrationParams getCameraIntrinsicsFromFile(std::string file) {
    SingleCalibrationParams calibrationParams;
    FileStorage fs(file, FileStorage::READ);

    fs["K"] >> calibrationParams.K;
    fs["D"] >> calibrationParams.D;

    return calibrationParams;
}


/*
 *
 */
std::vector<std::array<Mat, 2>> getImagesFromCamera(int numImages, 
        string ipAddr1, string ipAddr2) {

    std::vector<std::array<Mat, 2>> images;
    
    VideoCapture video1, video2;
    
    // Open video camera
    bool isOpenSuccessful1 = video1.open("rtsp://" + ipAddr1 + ":554/onvif1");
    bool isOpenSuccessful2 = video2.open("rtsp://" + ipAddr2 + ":554/onvif1");

    // Check for connection error
    if (!isOpenSuccessful1 || !isOpenSuccessful2) {
        // TODO: Specify which camera is not connecting
        throw std::runtime_error("Unable to conect to a camera");
    }

    // Wait for user input then get the image at that point
    std::cout << "Position chessboard and press enter to take picture..." << std::endl;
    std::array<Mat, 2> tempImgArray;
    for (int i=0; i<numImages; i++) {
        getchar();
        video1 >> tempImgArray.at(0);
        video2 >> tempImgArray.at(1);
        images.push_back(tempImgArray);
    }

    return images;
}


/* TODO: Implement getting images form files
 *
 */
std::vector<std::array<Mat, 2>> getImagesFromFile(string fileName) {
    std::vector<std::array<Mat, 2>> imagePairs;

    return imagePairs;
}


/*
 *
 */
StereoCalibrationParams stereoCalibrateWrapper(std::vector<std::array<Mat, 2>> imagePairs, 
        SingleCalibrationParams cam1CalibrationParams, 
        SingleCalibrationParams cam2CalibrationParams) {
    
    std::vector<std::vector<Point3f>> object_points;
    std::vector<std::vector<Point2f>> imagePoints1, imagePoints2;
    std::vector<Point2f> corners1, corners2;

    bool found1 = false, found2 = false;
    Mat gray1, gray2;
    for (const std::array<Mat, 2> &imgArr : imagePairs) {
        cvtColor(imgArr.at(0), gray1, CV_BGR2GRAY);
        cvtColor(imgArr.at(1), gray2, CV_BGR2GRAY);

        found1 = findChessboardCorners(imgArr.at(0), BOARD_SIZE, corners1, 
            CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
        found2 = findChessboardCorners(gray2, BOARD_SIZE, corners2, 
            CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);

        std::vector<Point3f> obj;
        for (int i=0; i<BOARD_HEIGHT; i++) {
            for (int j=0; j<BOARD_WIDTH; j++) {
                obj.push_back(Point3f((float) j*SQUARE_SIZE, (float) i*SQUARE_SIZE, 0));
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
    Vec3d T;

    stereoCalibrate(
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
    FileStorage fs(saveFile, FileStorage::WRITE);

    fs << "K1" << stereoCalibrationParams.cam1Intrinsics.K;
    fs << "D1" << stereoCalibrationParams.cam1Intrinsics.D;
    fs << "K2" << stereoCalibrationParams.cam2Intrinsics.K;
    fs << "D2" << stereoCalibrationParams.cam2Intrinsics.D;

    fs << "R" << stereoCalibrationParams.R;
    fs << "T" << stereoCalibrationParams.T;
    fs << "E" << stereoCalibrationParams.E;
    fs << "F" << stereoCalibrationParams.F;
}