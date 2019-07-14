#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <array>
#include <thread>
#include <chrono>
#include <stdio.h>
#include <stdexcept>

#define USE_CAMERA 2

#define CAM1_IP_ADDR "192.168.1.6"
#define CAM2_IP_ADDR "192.168.1.2"

#define FILE1_TEMPLATE "cameraSetup2/cam1/image*.png"
#define FILE2_TEMPLATE "cameraSetup2/cam2/image*.png"
#define REPLACE_STR "\\*"

#define OUT_FILE1 "cameraSetup2/cam1/calibResults"
#define OUT_FILE2 "cameraSetup2/cam2/calibResults"

#define BOARD_WIDTH 9        // Number of squares the checkerboard is wide
#define BOARD_HEIGHT 7        // Number of squares the checkerboard is high
#define BOARD_SIZE cv::Size(BOARD_WIDTH, BOARD_HEIGHT)
#define BOARD_N (BOARD_WIDTH * BOARD_HEIGHT)
#define SQUARE_SIZE 20    // Number of mills of each checkerboard size


/**
 *
 */
class UpdatingCamera {
private:
    cv::VideoCapture cap;
    cv::Mat frame;
    std::thread th;

    volatile bool stopFrameCaptureFlag = false;

public:
    UpdatingCamera(std::string ipAddress) {
        cap = cv::VideoCapture("rtsp://admin:A1b2c3d4e5@" + ipAddress + ":554/onvif1");

        openCapture();

        startListening();
    }

    ~UpdatingCamera() {
        stopFrameCaptureFlag = true;
        th.join();
    }

    bool openCapture() {
        if(!cap.isOpened()){
            std::cerr << "Error Opening Capture Device" << std::endl;
            return false;
        }

        return true;    
    }

    void startListening() {
        th = startListenThread();

    }

    std::thread startListenThread() {
        return std::thread(&UpdatingCamera::listenThread, this);
    }

    void listenThread() {
        while (!stopFrameCaptureFlag) {
            cv::Mat currFrame;
            cap.read(currFrame);
            //double timestamp = cap.get(CV_CAP_PROP_POS_MSEC);

            if (currFrame.empty()) {
                std::cout << "Empty" << std::endl;
            }
            else {
                frame = currFrame;
                //cap.set(CV_CAP_PROP_POS_FRAMES, 1.0);
            }
        }
    }

    cv::Mat getFrame() {
        return frame;
    }
};

std::vector<cv::Mat> getImagesFromCamera(int, std::string);
std::vector<std::array<cv::Mat, 2>> getStereoImagesFromCamera(int, std::string, std::string);
std::vector<cv::Mat> getImagesFromFile(std::string);
void saveImagesToFile(std::vector<cv::Mat>, std::string, std::string);
void saveStereoImagesToFile(std::vector<std::array<cv::Mat, 2>>, std::string, std::string, std::string);
bool calibrateCameraUsingImages(std::vector<cv::Mat>, std::string);
bool calibrateCameraUsingStereoImages(std::vector<std::array<cv::Mat, 2>>, std::string, std::string);

int main(int argc, char * argv[]) {
    std::vector<cv::Mat> calibImages1, calibImages2;
    std::vector<std::array<cv::Mat, 2>> calibStereoImages;

    int numImages = 0;

    std::cout << "Enter the number of images:";
    std::cin >> numImages;

    if (USE_CAMERA == 1) {
        //calibImages1 = getImagesFromCamera(numImages, CAM1_IP_ADDR);
        calibImages2 = getImagesFromCamera(numImages, CAM2_IP_ADDR);

        //saveImagesToFile(calibImages1, FILE1_TEMPLATE, REPLACE_STR);
        saveImagesToFile(calibImages2, FILE2_TEMPLATE, REPLACE_STR);
    }
    if (USE_CAMERA == 2) {
        calibStereoImages = getStereoImagesFromCamera(numImages, CAM1_IP_ADDR, CAM2_IP_ADDR);

        saveStereoImagesToFile(calibStereoImages, FILE1_TEMPLATE, FILE2_TEMPLATE, REPLACE_STR);
    }
    else {
        //calibImgaes1 = getImagesFromFile(FILE1_TEMPLATE, REPLACE_STR, NUM_IMAGES);
        //calibImgaes2 = getImagesFromFile(FILE2_TEMPLATE, REPLACE_STR, NUM_IMAGES);
    }

    //calibrateCameraUsingImages(calibImages1, OUT_FILE1);
    //calibrateCameraUsingImages(calibImages2, OUT_FILE2);

    calibrateCameraUsingStereoImages(calibStereoImages, OUT_FILE1, OUT_FILE2);

    return 0;
}


/*
 *
 */
std::vector<cv::Mat> getImagesFromCamera(int numImages, std::string ipAddr) {
    std::vector<cv::Mat> images;
    
    UpdatingCamera camera(ipAddr);
    camera.openCapture();

    // Wait for user input then get the image at that point
    std::cout << "Position chessboard and press enter to take picture..." << std::endl;
    int count=1;
    cv::Mat tempImg;
    for (int i=0; i<numImages; i++) {
        std::cout << "Setup camera" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        //std::cin >> count;
        std::cin.clear();
        tempImg = camera.getFrame();
        images.push_back(tempImg);
    }

    return images;
}

/*
 *
 */
std::vector<std::array<cv::Mat, 2>> getStereoImagesFromCamera(int numImages, std::string ipAddr1, std::string ipAddr2) {
    std::vector<std::array<cv::Mat, 2>> images;
    
    UpdatingCamera camera1(ipAddr1);
    UpdatingCamera camera2(ipAddr2);
    camera1.openCapture();
    camera2.openCapture();

    // Wait for user input then get the image at that point
    std::cout << "Position chessboard and press enter to take picture..." << std::endl;
    int count=1;
    cv::Mat tempImg1, tempImg2;
    for (int i=0; i<numImages; i++) {
        std::cout << "Setup camera" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        //std::cin >> count;
        std::cin.clear();
        tempImg1 = camera1.getFrame();
        tempImg2 = camera2.getFrame();
        std::array<cv::Mat, 2> tempArray {{tempImg1, tempImg2}};
        images.push_back(tempArray);
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
void saveStereoImagesToFile(std::vector<std::array<cv::Mat, 2>> images, std::string filePattern1, 
        std::string filePattern2, std::string replaceStr) {

    std::regex r(replaceStr);
    std::string fileName1, fileName2;
    for (int i=0; i<images.size(); i++) {
        fileName1 = std::regex_replace (filePattern1, r, std::to_string(i));
        fileName2 = std::regex_replace (filePattern2, r, std::to_string(i));

        cv::imwrite(fileName1, images[i][0]);
        cv::imwrite(fileName2, images[i][1]);
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

/*
 *
 */
bool calibrateCameraUsingStereoImages(std::vector<std::array<cv::Mat, 2>> images, 
        std::string outFile1, std::string outFile2) {
    
    std::vector<cv::Mat> images1, images2;
    for (int i=0; i<images.size(); i++) {
        images1.push_back(images[i][0]);
        images2.push_back(images[i][1]);
    }

    calibrateCameraUsingImages(images1, outFile1);
    calibrateCameraUsingImages(images2, outFile2);

    return true;
}