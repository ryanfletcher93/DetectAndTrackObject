#include <opencv2/opencv.hpp>

#include "Camera.h"
#include "Detector.h"

class Runner {
	Runner() {

	}
};

int main() {
	Camera* cam1 = new IpCamera(); 	
	Camera* cam2 = new IpCamera();

	cv::Mat backgroundImg1, backgroundImg2;

	StereoCamera * stereoCamera1 = new StereoCameraWithBackground(cam1, cam2, backgroundImg1, backgroundImg2);


	Detector* thresholdDetector = new ThresholdDetector();


	return 0;
}




/*
class Runner {
private:
Mat img1, img2;
StereoCamera* stereoCamera;
Detector* detector;
Tracker* tracker;
Trigger* trigger;

public:
	Runner() {
	}



	void runTracking() {
		detector = new ThresholdDetector();
		tracker = new KalmanTracker();
		trigger = new Trigger();

		while(true) {
			std::array<Mat, 2> = stereoCamera->getImages();

			detector.detect(stereoCamera);

			tracker.track(detector);

			if (false) {
				trigger.fire();
			}
		}
	}
}
*/