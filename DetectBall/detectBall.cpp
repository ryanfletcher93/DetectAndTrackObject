/**
 Copyright 2017 by Satya Mallick ( Big Vision LLC )
 http://www.learnopencv.com
 **

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

struct ColorRange {
    int hueLow;
    int hueHigh;

    int spectrumLow;
    int spectrumHigh;

    int valueLow;
    int valueHigh;
}

void getFrame(Mat * image) {
    // TODO: get image
    image = new Mat();
}

int main(int argc, char** argv )
{
    Mat currentImage;

    while (true) {
        getFrame(currentImage);
    }

    // Read image
    Mat img = imread("red_eyes2.jpg",CV_LOAD_IMAGE_COLOR);

    int iLowH1 = 160;
    int iHighH1 = 179;

    int iLowH2 = 0;
    int iHighH2 = 10;

    int iLowS = 150; 
    int iHighS =200;

    int iLowV = 150;
    int iHighV = 200;
    
    Mat imgHSV;
    
    cvtColor(img, imgHSV, COLOR_BGR2HSV);

    Mat imgThreshHold1;
    Mat imgThreshHold2;
    inRange(imgHSV, Scalar(iLowH1, iLowS, iLowV), Scalar(iHighH1, iHighS, iHighV), imgThreshHold1);
    inRange(imgHSV, Scalar(iLowH2, iLowS, iLowV), Scalar(iHighH2, iHighS, iHighV), imgThreshHold2);

    Mat combined_image;
    addWeighted(imgThreshHold1, 1.0, imgThreshHold2, 1.0, 0.0, combined_image);
}
*/

#include <iostream>
#include <memory>

//using namespace std;

void printValues(const int& a) {
    std::cout << "Inside function a :" << &a << std::endl;
}

class ABC {
public:
    ABC() {

    }
};

int main(int argc, char** argv) {

    int a = 123;

    std::cout << "Before funciton a: " << &a << std::endl;

    printValues(a);

    std::cout << "a: " << a << std::endl;

    {
        std::unique_ptr<int> test =  std::make_unique<int>(42);
        std::cout << *test << std::endl;
    }

    return 0;
}