#include <opencv2/opencv.hpp>

#include "Camera.h"
#include "Detector.h"
#include "StereoCamera.h"
#include "Tracker.h"

class Runner {
private:
	StereoCamera * stereoCamera;
	Detector * detector;
	Tracker * tracker;


public:
	Runner() {
		Camera* cam1 = new VideoFromFile("", "../src/calibration/cameraSetup1/cam1/calibResults"); 	
		Camera* cam2 = new VideoFromFile("", "../src/calibration/cameraSetup1/cam2/calibResults");

		cv::Mat backgroundImg1, backgroundImg2;

		stereoCamera = new StereoCameraWithBackground(cam1, cam2, "../src/calibration/cameraSetup1/stereo/calibResults");

		detector = new BackgroundSubtractionDetector(stereoCamera);

		tracker = new KalmanTracker();
	}

	void startTracking() {
		int numRemainingImages = 1;
		while (numRemainingImages >= 0) {
			stereoCamera->getImageFromCameras();
			DetectorResults* detectorResults = detector->detect();

			tracker->updateTracking(detectorResults);

			delete detectorResults;

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
