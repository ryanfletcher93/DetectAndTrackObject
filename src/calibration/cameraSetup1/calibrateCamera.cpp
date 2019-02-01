#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <array>
#include <stdexcept>

#define USE_CAMERA 1

#define CAM1_IP_ADDR "192.168.1.7"
#define CAM2_IP_ADDR "192.168.1.9"

#define FILE1_TEMPLATE "cam1/image*.png"
#define FILE2_TEMPLATE "cam2/image*.png"
#define REPLACE_STR "\\*"

#define OUT_FILE1 "cam1/calibResults"
#define OUT_FILE2 "cam2/calibResults"

#define BOARD_WIDTH 9        // Number of squares the checkerboard is wide
#define BOARD_HEIGHT 7        // Number of squares the checkerboard is high
#define BOARD_SIZE cv::Size(BOARD_WIDTH, BOARD_HEIGHT)
#define BOARD_N (BOARD_WIDTH * BOARD_HEIGHT)
#define SQUARE_SIZE 20    // Number of mills of each checkerboard size


std::vector<cv::Mat> getImagesFromCamera(int, std::string);
std::vector<cv::Mat> getImagesFromFile(std::string);
void saveImagesToFile(std::vector<cv::Mat>, std::string, std::string);
bool calibrateCameraUsingImages(std::vector<cv::Mat>, std::string);

int main(int argc, char * argv[]) {
    std::vector<cv::Mat> calibImages1, calibImages2;

    int numImages = 0;

    std::cout << "Enter the number of images:";
    std::cin >> numImages;

    if (USE_CAMERA) {
        //calibImages1 = getImagesFromCamera(numImages, CAM1_IP_ADDR);
        calibImages2 = getImagesFromCamera(numImages, CAM2_IP_ADDR);

        //saveImagesToFile(calibImages1, FILE1_TEMPLATE, REPLACE_STR);
        saveImagesToFile(calibImages2, FILE2_TEMPLATE, REPLACE_STR);
    }
    else {
        //calibImgaes1 = getImagesFromFile(FILE1_TEMPLATE, REPLACE_STR, NUM_IMAGES);
        //calibImgaes2 = getImagesFromFile(FILE2_TEMPLATE, REPLACE_STR, NUM_IMAGES);
    }

    //calibrateCameraUsingImages(calibImages1, OUT_FILE1);
    calibrateCameraUsingImages(calibImages2, OUT_FILE2);

    return 0;
}


/*
 *
 */
std::vector<cv::Mat> getImagesFromCamera(int numImages, std::string ipAddr) {
    std::vector<cv::Mat> images;
    
    cv::VideoCapture video;
    
    // Open video camera
    bool isOpenSuccessful = video.open("rtsp://" + ipAddr + ":554/onvif1");

    // Check for connection error
    if (!isOpenSuccessful) {
        // TODO: Specify which camera is not connecting
        throw std::runtime_error("Unable to conect to a camera");
    }

    // Wait for user input then get the image at that point
    std::cout << "Position chessboard and press enter to take picture..." << std::endl;
    int count=1;
    cv::Mat tempImg;
    for (int i=0; i<numImages; i++) {
        //std::cout << i;
        std::cin >> count;
        std::cin.clear();
        video >> tempImg;
        images.push_back(tempImg);
    }

    return images;
}


/* TODO: Implement getting images form files
 *
 */
std::vector<cv::Mat> getImagesFromFile(std::string filePattern, 
        std::string replaceSymbol, int numFiles) {

    std::regex r (replaceSymbol);
    std::vector<cv::Mat> images;

    for (int i=0; i<numFiles; i++) {
        std::string fileName = std::regex_replace (filePattern, r, std::to_string(i));
        cv::Mat tempImage = cv::imread(fileName, CV_LOAD_IMAGE_COLOR);

        images.push_back(tempImage);
        if (!tempImage.data) {
            std::cout << "Could not open or find the image";
            std::cout << filePattern << ": " << i << std::endl;
        }
    }

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


/*
 *
 */
bool calibrateCameraUsingImages(std::vector<cv::Mat> images, std::string outFile) {
    
    std::vector<std::vector<cv::Point3f>> object_points;
    std::vector<std::vector<cv::Point2f>> imagePoints;
    std::vector<cv::Point2f> corners;

    /* For each calibration image, if can find checkerboard, 
    add points to calibration set */
    bool found = false;
    cv::Mat gray;
    int count = 1;
    for (const cv::Mat &img : images) {
        cv::cvtColor(img, gray, CV_BGR2GRAY);

        found = cv::findChessboardCorners(gray, BOARD_SIZE, corners, 
            CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
        
        std::vector<cv::Point3f> obj;
        for (int i=0; i<BOARD_HEIGHT; i++) {
            for (int j=0; j<BOARD_WIDTH; j++) {
                obj.push_back(cv::Point3f((float) j*SQUARE_SIZE, (float) i*SQUARE_SIZE, 0));
            }
        }

        std::cout << found << std::endl;
        if (found) {
            std::cout << "Found chessboard" << std::endl;
            object_points.push_back(obj);
            imagePoints.push_back(corners);
        }
    }

    // Get calibration parameters using checkboard points
    cv::Mat K,D;
    std::vector<cv::Mat> rvecs, tvecs;
    int flag=0;
    flag |= CV_CALIB_FIX_K4;
    flag |= CV_CALIB_FIX_K5;

    if (object_points.size() == 0 || imagePoints.size() == 0) {
        std::cout << "No boards detected, nothing written to " << outFile << std::endl;
        return false;
    }

    cv::calibrateCamera(object_points, imagePoints, gray.size(), K, D, rvecs, tvecs, flag);

    // Store calibration matricies to file
    cv::FileStorage fs(outFile, cv::FileStorage::WRITE);
    fs << "K" << K;
    fs << "D" << D;
    fs << "BOARD_WIDTH" << BOARD_WIDTH;
    fs << "BOARD_HEIGHT" << BOARD_HEIGHT;
    fs << "SQUARE_SIZE" << SQUARE_SIZE;

    return true;
}