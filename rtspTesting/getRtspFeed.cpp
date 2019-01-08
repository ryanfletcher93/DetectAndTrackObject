#include <opencv2/opencv.hpp>
#include <typeinfo>

using namespace cv;

#define IS_VIDEO 1

class DetectedObject {
private:
	int objectCentre[2];

public:
	DetectedObject() {
		
	}

};

class Environment {
private:
	std::vector<DetectedObject> objects;

public:
	void updateEnvironment(const std::vector<KeyPoint> &keypoints) {

	}
};

/**
 *
 */
std::vector<KeyPoint> detectObjectsByColour(const Mat &image, Mat &detectedImage) {
	
	// Define thresholds for 
	Scalar minThresh = Scalar(20, 150, 100);
	Scalar maxThresh = Scalar(40, 255, 255);

	//medianBlur(image, image, 5);
	Mat convertedImage, thresholdImage;

	cvtColor(image, convertedImage, CV_BGR2HSV);

	inRange(convertedImage, minThresh, maxThresh, thresholdImage);
	bitwise_not (thresholdImage, thresholdImage);

	detectedImage = thresholdImage;

	SimpleBlobDetector::Params params;
	// thresholds
	params.filterByColor = true;
	params.minThreshold = 10;
	params.maxThreshold = 200;
	//params.thresholdStep = 20

	// filter by area
	params.filterByArea = true;
	params.minArea = 50;
	params.maxArea = 10000;

	// filter by circularity
	params.filterByCircularity = false;
	// filter by convexity
	params.filterByConvexity = false;

	// filter by inertia
	params.filterByInertia = false;

	std::vector<KeyPoint> keypoints;
	SimpleBlobDetector detector(params);

	detector.detect(thresholdImage, keypoints);
	
	if (!keypoints.empty()) {
		//std::cout << keypoints[0].size << std::endl;
	}

	Mat im_with_keypoints;
	drawKeypoints(thresholdImage, keypoints, im_with_keypoints, Scalar(255,0,0), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );


	return keypoints;
}

void detectObjectsByBackgroundImage(const Mat &image, 
		const Mat &backgroundImage, 
		Mat &detectedImage) {

	Mat diffImage;
	cv::subtract(image, backgroundImage, diffImage);

	Scalar minThresh = Scalar(-10, 0, 0);
	Scalar maxThresh = Scalar(10, 255, 255);

	inRange(diffImage, minThresh,maxThresh , detectedImage);
	bitwise_not(detectedImage, detectedImage);
}

/**
 *
 */
int main(int argc, int * argv[]) {

	VideoCapture video;
	
	bool isOpenSuccessful;

	if (IS_VIDEO) {
		isOpenSuccessful = video.open("/home/ryan/Documents/Prog Projects/ImageProcessing/rtspTesting/boundingTennisBall.avi");
	}
	else {
		isOpenSuccessful = video.open("rtsp://192.168.1.2:554/onvif1");
	}

	if (!isOpenSuccessful) {
		return 1;
	}

	int width = video.get(CV_CAP_PROP_FRAME_WIDTH);
	int height = video.get(CV_CAP_PROP_FRAME_HEIGHT);

	std::cout << "Width: " << width << ", Height: " << height << std::endl;

	Mat image, returnedImage, backgroundImage;
	std::vector<KeyPoint> keypoints;
	Environment environment;
	
	for (int i=0; i<video.get(CV_CAP_PROP_FRAME_COUNT); i++) {
		// Get image frame
		video.read(image);

		// Get background image
		if (i <= 15) {
			backgroundImage = image.clone();
			continue;
		}

		detectObjectsByBackgroundImage(image, backgroundImage, returnedImage);

		std::cout << i << std::endl;

		/*
		if (keypoints.size() != 0) {
			std::cout << "x: " << keypoints[0].pt.x << ", y: " << keypoints[0].pt.y << std::endl;
		}
		*/

		//environment.updateEnvironment(keypoints);

		imshow("Display Window", returnedImage);

		char c = (char) waitKey(1);
		if (c == 27) {
			break;
		}
	}

	video.release();
	destroyAllWindows();

	return 0;
}
