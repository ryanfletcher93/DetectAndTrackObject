#include <opencv2/opencv.hpp>

using namespace cv;

class Runner {
	Runner() {

	}
};

int main() {

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