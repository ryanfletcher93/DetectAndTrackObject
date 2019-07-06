#include <opencv2/opencv.hpp>

#include "Camera.h"
#include "Detector.h"
#include "StereoCamera.h"
#include "Tracker.h"
#include "Calibration.h"
#include "Motor.h"

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
	StereoParameters * stereoParameters;
	StereoCamera * stereoCamera;
	Detector * detector;
	Tracker * tracker;
	ServoMotor * motor;

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

			// Do not do this more than once as will keep appending same data to file
			//saveStereoParameters(R1, R2, P1, P2, Q);

			stereoParameters = new StereoParameters(R1, R2, P1, P2, Q);

			std::vector<cv::Vec2f> coords1, coords2;
			for (auto it = kp1.begin(); it!=kp1.end(); it++) {
			    coords1.push_back(it->pt);
			}
			for (auto it = kp2.begin(); it!=kp2.end(); it++) {
			    coords2.push_back(it->pt);
			}
			
			cv::Vec2f c1l(1200.0f, 555.0f);
			cv::Vec2f c1r(200.0f, 555.0f);
			coords1.push_back(c1l);
			coords2.push_back(c1r);

			cv::Mat res;
			cv::triangulatePoints(P1, P2, coords1, coords2, res);

			cv::Vec4d triangCoords = res.col(0);
			cv::Vec4d col2 = res.col(1);

			cv::Point3f homogeneousCoord;
			homogeneousCoord.x = triangCoords[0] / triangCoords[3];
			homogeneousCoord.y = triangCoords[1] / triangCoords[3];
			homogeneousCoord.z = triangCoords[2] / triangCoords[3];

			for (int i=0; i<3; i++) {
				std::cout << (col2[i] / col2[3]) << std::endl;
			}

			AngleCalculator ac;
			float panAngle = ac.getPanAngle(homogeneousCoord, 0);
			float tiltAngle = ac.getTiltAngle(homogeneousCoord, 0, 0);

			motor = new ServoMotor();
			motor->moveMotorToPosition(115.0f, 115.0f);
			motor->moveMotorToPosition(135.0f, 135.0f);
			motor->moveMotorToPosition(120.0f, 120.0f);
			motor->moveMotorToPosition(105.0f, 105.0f);
			motor->moveMotorToPosition(90.0f, 90.0f);
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

	void SendAnglesToPanTiltMechanism(float panAngle, float tiltAngle) {

	}
};


int main() {

	Runner runner;
	runner.performAction(ActionType::DetectColour);

	std::cout << &runner << std::endl;

	return 0;
}
