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

enum class ActionType {DetectColour, DetectShape, DetectAndTrack};

class Runner {
private:
	StereoCamera * stereoCamera;
	Detector * detector;
	Tracker * tracker;

	Camera* cam1;
	Camera* cam2;


public:
	Runner() {
		InputType inputType = InputType::Image;

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
	}

	//
	void performAction(ActionType actionType) {
		if (actionType == ActionType::DetectColour) {
			cv::Mat image1 = cam1->getImage();
			cv::Mat image2 = cam2->getImage();

			cv::Scalar minRedThreshold = cv::Scalar(150, 110, 90);
			cv::Scalar maxRedThreshold = cv::Scalar(180, 255, 255);

			cv::Scalar minYellowThresh = cv::Scalar(25, 112, 44);
			cv::Scalar maxYellowThresh = cv::Scalar(50, 230, 225);

			std::vector<cv::KeyPoint> kp1, kp2;
			ColourDetector colourDetector;
			//kp1 = colourDetector.identifyObjectsByColour(image1, minRedThreshold, maxRedThreshold);
			//kp2 = colourDetector.identifyObjectsByColour(image2, minRedThreshold, maxRedThreshold);
			kp1 = colourDetector.identifyObjectsByColour(image1, minYellowThresh, maxYellowThresh);
			kp2 = colourDetector.identifyObjectsByColour(image2, minYellowThresh, maxYellowThresh);

			cv::Mat K1, K2;
			cv::Mat D1, D2;
			getCameraIntrinsics(K1, D1, K2, D2);

			cv::Mat R;
			cv::Vec3d T;
			getStereoCameraIntrinsics(R, T);

			cv::Mat R1, R2, P1, P2, Q;
			cv::stereoRectify(K1, D1, K2, D2, image1.size(), R, T, R1, R2, P1, P2, Q);

			saveStereoParameters(R1, R2, P1, P2, Q);

			std::vector<cv::Vec2f> coords1, coords2;
			for (auto it = kp1.begin(); it!=kp1.end(); it++) {
			    coords1.push_back(it->pt);
			}
			for (auto it = kp2.begin(); it!=kp2.end(); it++) {
			    coords2.push_back(it->pt);
			}

			cv::Mat res;
			cv::triangulatePoints(P1, P2, coords1, coords2, res);

			cv::Vec4d triangCoords = res.col(0);

			for (int i=0; i<3; i++) {
				std::cout << (triangCoords[i] / triangCoords[3]) << std::endl;
			}
		}
		else if (actionType == ActionType::DetectAndTrack) {
			stereoCamera = new StereoCameraWithBackground(cam1, cam2, "../src/calibration/cameraSetup1/stereo/calibResults");
			stereoCamera->setStereoIntrinsicsFromFile(STEREO_INTRINSICS_PATH);

			//
			Mat backgroundImg1 = cv::imread(BACKGROUND_IMG_1);
			Mat backgroundImg2 = cv::imread(BACKGROUND_IMG_2);
			detector = new BackgroundSubtractionDetector(stereoCamera);
			detector->setBackgroundImages(backgroundImg1, backgroundImg2);

			tracker = new KalmanTracker();
		}
	}


	void getCameraIntrinsics(cv::Mat& K1, cv::Mat& D1, cv::Mat& K2, cv::Mat& D2) {
		std::string calibPath = "/home/ryan/Documents/Prog Projects/ImageProcessing/src/calibration/cameraSetup1/";
		std::string fileCam1 = calibPath + "cam1/calibResults";
		cv::FileStorage fs1(fileCam1, cv::FileStorage::READ);

		fs1["K"] >> K1;
		fs1["D"] >> D1;

		std::string fileCam2 = calibPath + "cam2/calibResults";
		cv::FileStorage fs2(fileCam2, cv::FileStorage::READ);

		fs2["K"] >> K2;
		fs2["D"] >> D2;
	}

	void getStereoCameraIntrinsics(cv::Mat& R, cv::Vec3d& T) {
		std::string calibPath = "/home/ryan/Documents/Prog Projects/ImageProcessing/src/calibration/cameraSetup1/stereo/StereoCameraProperties";
		cv::FileStorage fs(calibPath, cv::FileStorage::READ);

		fs["R"] >> R;
		fs["T"] >> T;
	}

	void saveStereoParameters(cv::Mat R1, cv::Mat R2, cv::Mat P1, cv::Mat P2, cv::Mat Q) {
		std::string calibPath = "/home/ryan/Documents/Prog Projects/ImageProcessing/src/calibration/cameraSetup1/stereo/StereoCameraProperties";
		cv::FileStorage fs(calibPath, cv::FileStorage::APPEND);

		fs << "R1" << R1;
		fs << "R2" << R2;
		fs << "P1" << P1;
		fs << "P2" << P2;
		fs << "Q" << Q;
	}


	/*
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
	*/
};


int main() {

	Runner runner;
	runner.performAction(ActionType::DetectColour);

	std::cout << &runner << std::endl;

	return 0;
}
