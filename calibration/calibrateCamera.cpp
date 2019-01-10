#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <stdexcept>

using namespace cv;

#define USE_CAMERA 1

#define BOARD_WIDTH 4        // Number of squares the checkerboard is wide
#define BOARD_HEIGHT 4        // Number of squares the checkerboard is high
#define BOARD_SIZE Size(BOARD_WIDTH, BOARD_HEIGHT)
#define BOARD_N (BOARD_WIDTH * BOARD_HEIGHT)
#define SQUARE_SIZE 24.23    // Number of mills of each checkerboard size

std::vector<Mat> getImagesFromCamera(int, string);
std::vector<Mat> getImagesFromFile(string);

int main(int argc, char * argv[]) {
    std::vector<Mat> images;
    if (USE_CAMERA) {
        //images = getImagesFromCamera(1, "192.168.1.2");
    }
    else {
        //images = getImagesFromFile();
    }

    //calibrateCameraUsingImages(images);

    return 0;
}


/*
 *
 */
std::vector<Mat> getImagesFromCamera(int numImages, string ipAddr) {
    std::vector<Mat> images;
    
    VideoCapture video;
    
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
    Mat tempImg;
    for (int i=0; i<numImages; i++) {
        std::cout << i;
        getchar();
        video >> tempImg;
        images.push_back(tempImg);
    }

    return images;
}


/* TODO: Implement getting images form files
 *
 */
std::vector<Mat> getImagesFromFile(string fileName) {
    std::vector<Mat> imagePairs;

    return imagePairs;
}


/*
 *
 */
void calibrateCameraUsingImages(std::vector<Mat> images) {
    
    std::vector<std::vector<Point3f>> object_points;
    std::vector<std::vector<Point2f>> imagePoints;
    std::vector<Point2f> corners;

    /* For each calibration image, if can find checkerboard, 
    add points to calibration set */
    bool found = false;
    Mat gray;
    for (const Mat &img : images) {
        cvtColor(img, gray, CV_BGR2GRAY);

        found = findChessboardCorners(img, BOARD_SIZE, corners, 
            CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
        
        std::vector<Point3f> obj;
        for (int i=0; i<BOARD_HEIGHT; i++) {
            for (int j=0; j<BOARD_WIDTH; j++) {
                obj.push_back(Point3f((float) j*SQUARE_SIZE, (float) i*SQUARE_SIZE, 0));
            }
        }

        if (found) {
            std::cout << "Found chessboard" << std::endl;
            object_points.push_back(obj);
            imagePoints.push_back(corners);
        }
    }

    // Get calibration parameters using checkboard points
    Mat K,D;
    std::vector<Mat> rvecs, tvecs;
    int flag=0;
    flag |= CV_CALIB_FIX_K4;
    flag |= CV_CALIB_FIX_K5;

    calibrateCamera(object_points, imagePoints, gray.size(), K, D, rvecs, tvecs, flag);

    // Store calibration matricies to file
    char* outFile;
    FileStorage fs(outFile, FileStorage::WRITE);
    fs << "K" << K;
    fs << "D" << D;
    fs << "BOARD_WIDTH" << BOARD_WIDTH;
    fs << "BOARD_HEIGHT" << BOARD_HEIGHT;
    fs << "SQUARE_SIZE" << SQUARE_SIZE;


    return;
}