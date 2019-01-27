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
		Camera* cam1 = new IpCamera(""); 	
		Camera* cam2 = new IpCamera("");

		cv::Mat backgroundImg1, backgroundImg2;

		stereoCamera = new StereoCameraWithBackground(cam1, cam2, backgroundImg1, backgroundImg2);

		detector = new BackgroundSubtractionDetector(stereoCamera);

		tracker = new KalmanTracker();
	}

	void startTracking() {
		int numRemainingImages = 1;
		while (numRemainingImages >= 0) {
			stereoCamera->getImages();
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
