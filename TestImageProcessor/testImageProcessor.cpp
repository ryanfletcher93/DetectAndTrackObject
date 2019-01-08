/**
 Copyright 2017 by Satya Mallick ( Big Vision LLC )
 http://www.learnopencv.com
**/

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv )
{
    // Read image
    Mat img = imread("red_eyes2.jpg",CV_LOAD_IMAGE_COLOR);

    // Output image
    Mat imgOut = img.clone();

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


    // Display Result
    imshow("Red Eyes", img);
    imshow("Red Eyes Removed", combined_image);
    waitKey(0);

}
