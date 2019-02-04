#include <opencv2/opencv.hpp>

#include "Camera.h"
#include "Detector.h"
#include "StereoCamera.h"
#include "Tracker.h"

#define CAM1_IPADDR "192.168.1.7"
#define CAM2_IPADDR "192.168.1.9"

#define CAM1_INTRINSICS_PATH "../src/calibration/cameraSetup1/cam1/calibResults"
#define CAM2_INTRINSICS_PATH "../src/calibration/cameraSetup1/cam2/calibResults"

#define CAM1_VIDEO_PATH "../src/calibration/cameraSetup1/cam1/video0.avi"
#define CAM2_VIDEO_PATH "../src/calibration/cameraSetup1/cam2/video0.avi"

#define IMAGE1_PATH "../src/calibration/cameraSetup1/cam1/background1WithObjects.png"
#define IMAGE2_PATH "../src/calibration/cameraSetup1/cam2/background1WithObjects.png"

#define STEREO_INTRINSICS_PATH "../src/calibration/cameraSetup1/stereo/StereoCameraProperties"

#define BACKGROUND_IMG_1 "../src/calibration/cameraSetup1/cam1/background1.png"
#define BACKGROUND_IMG_2 "../src/calibration/cameraSetup1/cam2/background1.png"

using cv::Mat;

enum class InputType {Camera,Video,Image};

class Runner {
private:
	StereoCamera * stereoCamera;
	Detector * detector;
	Tracker * tracker;


public:
	Runner() {
		InputType inputType = InputType::Image;

		Camera* cam1;
		Camera* cam2;

		if (inputType == InputType::Camera) {
			cam1 = new IpCamera(CAM1_IPADDR, CAM1_INTRINSICS_PATH);
			cam2 = new IpCamera(CAM2_IPADDR, CAM2_INTRINSICS_PATH);
		}
		else if (inputType == InputType::Video) {
			cam1 = new VideoFromFile(CAM1_VIDEO_PATH, CAM1_INTRINSICS_PATH);
			cam2 = new VideoFromFile(CAM2_VIDEO_PATH, CAM2_INTRINSICS_PATH);
		}
		else if (inputType == InputType::Image) {
			cam1 = new SingleImage(IMAGE1_PATH, CAM1_INTRINSICS_PATH);
			cam2 = new SingleImage(IMAGE2_PATH, CAM2_INTRINSICS_PATH);
		}

		stereoCamera = new StereoCameraWithBackground(cam1, cam2, "../src/calibration/cameraSetup1/stereo/calibResults");
		stereoCamera->setStereoIntrinsicsFromFile(STEREO_INTRINSICS_PATH);

		//
		Mat backgroundImg1 = cv::imread(BACKGROUND_IMG_1);
		Mat backgroundImg2 = cv::imread(BACKGROUND_IMG_2);
		detector = new BackgroundSubtractionDetector(stereoCamera);
		detector->setBackgroundImages(backgroundImg1, backgroundImg2);

		tracker = new KalmanTracker();
	}

	void startTracking() {
		int numRemainingImages = 1;
		while (numRemainingImages >= 0) {
			stereoCamera->getImageFromCameras();
			DetectorResults* detectorResults = detector->detect();

			//tracker->updateTracking(detectorResults);

			//delete detectorResults;

			numRemainingImages--;
		}
	}
};



int main() {

	Runner runner;
	runner.startTracking();

	std::cout << &runner << std::endl;

	return 0;
}
