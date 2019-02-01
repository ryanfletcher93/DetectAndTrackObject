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

#define VID1_OUT_PATH "cam1/video0.avi"
#define VID2_OUT_PATH "cam2/video0.avi"

#define BOARD_WIDTH 9        // Number of squares the checkerboard is wide
#define BOARD_HEIGHT 7        // Number of squares the checkerboard is high
#define BOARD_SIZE cv::Size(BOARD_WIDTH, BOARD_HEIGHT)
#define BOARD_N (BOARD_WIDTH * BOARD_HEIGHT)
#define SQUARE_SIZE 20    // Number of mills of each checkerboard size

void createVideoAndWriteToFile(int, std::string, std::string);

int main(int argc, char * argv[]) {

    createVideoAndWriteToFile(150, CAM1_IP_ADDR, CAM2_IP_ADDR);

    return 0;
}


/*
 *
 */
void createVideoAndWriteToFile(int numFrames, std::string ipAddr1, std::string ipAddr2) {
	cv::VideoCapture cap1("rtsp://" + ipAddr1 + ":554/onvif1");
	cv::VideoCapture cap2("rtsp://" + ipAddr2 + ":554/onvif1");

	if (!cap1.isOpened()) {
		std::cout << "Cam 1 is not opened" << std::endl;
	}
	if (!cap2.isOpened()) {
		std::cout << "Cam 2 is not opened" << std::endl;
	}

	int frame_width = cap1.get(CV_CAP_PROP_FRAME_WIDTH);
	int frame_height = cap2.get(CV_CAP_PROP_FRAME_HEIGHT);
   
    // Define the codec and create VideoWriter object.The output is stored in 'outcpp.avi' file. 
    cv::VideoWriter vid1 (VID1_OUT_PATH, CV_FOURCC('M','J','P','G'), 10, cv::Size(frame_width,frame_height));
    cv::VideoWriter vid2 (VID2_OUT_PATH, CV_FOURCC('M','J','P','G'), 10, cv::Size(frame_width,frame_height));
  	
  	for (int i=0; i<numFrames; i++) {
  		cv::Mat frame1, frame2;
  		cap1 >> frame1;
  		cap2 >> frame2;

  		vid1.write(frame1);
  		vid2.write(frame2);
  	}

  	cap1.release();
  	cap2.release();
  	vid1.release();
  	vid2.release();
}