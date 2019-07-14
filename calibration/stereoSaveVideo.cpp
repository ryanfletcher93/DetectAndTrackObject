#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <array>
#include <thread>
#include <stdexcept>

#define USE_CAMERA 1

#define CAM1_IP_ADDR "192.168.1.6"
#define CAM2_IP_ADDR "192.168.1.2"

#define VID1_OUT_PATH "cameraSetup2/cam1/video2.avi"
#define VID2_OUT_PATH "cameraSetup2/cam2/video2.avi"

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
            std::cout << "Error Opening Capture Device" << std::endl;
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

    int getFrameWidth() {
      return cap.get(CV_CAP_PROP_FRAME_WIDTH);
    }

    int getFrameHeight() {
      return cap.get(CV_CAP_PROP_FRAME_HEIGHT);
    }
};



/*
 *
 */
void createVideoAndWriteToFile(int numFrames, std::string ipAddr1, std::string ipAddr2) {
	UpdatingCamera cap1(ipAddr1);
  UpdatingCamera cap2(ipAddr2);

  //cap1.startListening();
  //cap2.startListening();

  /*
	if (!cap1.isOpened()) {
		std::cout << "Cam 1 is not opened" << std::endl;
	}
	if (!cap2.isOpened()) {
		std::cout << "Cam 2 is not opened" << std::endl;
	}
  */

	int frame_width = cap1.getFrameWidth();
	int frame_height = cap2.getFrameHeight();
   
    // Define the codec and create VideoWriter object.The output is stored in 'outcpp.avi' file. 
    cv::VideoWriter vid1 (VID1_OUT_PATH, CV_FOURCC('M','J','P','G'), 20, cv::Size(frame_width,frame_height));
    cv::VideoWriter vid2 (VID2_OUT_PATH, CV_FOURCC('M','J','P','G'), 20, cv::Size(frame_width,frame_height));
  	
    cv::Mat frame1, frame2;
  	for (int i=0; i<numFrames; i++) {
  		frame1 = cap1.getFrame();
      frame2 = cap2.getFrame();

  		vid1.write(frame1);
  		vid2.write(frame2);
  	}
}