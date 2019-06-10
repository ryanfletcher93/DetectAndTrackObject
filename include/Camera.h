#ifndef CAMERA_H
#define CAMERA_H

#include <opencv2/opencv.hpp>

using std::string;
using std::vector;

using cv::Mat;
using cv::VideoCapture;

/**
 *
 */
class Camera {
private:

protected:
	VideoCapture video;
	string cameraIntrinsicsFileName;

	Mat K, D;
	int boardWidth, boardHeight;
	float squareSize;

	virtual bool openCamera() = 0;
	virtual bool closeCamera() = 0;

public:
	virtual Mat getImage() = 0;

	virtual Mat getSingleImage();
	virtual Mat getImageOnButtonPress();
	virtual vector<Mat> getImagesOnButtonPress(int numImages);
	virtual vector<Mat> getImagesUsingDelay(int numImages, int delay);
	virtual vector<Mat> getImagesUntilCharacter(char stopChar);

	virtual void getCameraIntrinsicsFromFile(string filePath);
};

/**
 *
 */
class IpCamera : public Camera {
private:
	string ipAddr;

protected:
	bool openCamera();
	bool closeCamera();

public:
	IpCamera(string ipAddr, string intrinsicsFilePath);

	Mat getImage();

	string getIpAddr() {
		return this->ipAddr;
	}
	void setIpAddr(string ipAddr) {
		this->ipAddr = ipAddr;
	}
};

/**
 *
 */
class VideoFromFile : public Camera {
private:
	string fileName;

protected:
	bool openCamera();
	bool closeCamera();

public:
	VideoFromFile(string videoFilePath, string intrinsicsFilePath);

	Mat getImage();

	string getFileName() {
		return this->fileName;
	}
	void setFileName(string fileName) {
		this->fileName = fileName;
	}
};


/**
 *
 */
class SingleImage : public Camera {
private:
	string filePath;

protected:
	bool openCamera();
	bool closeCamera();

public:
	/*
	 */
	SingleImage(string imageFilePath, string intrinsicsFilePath);

	Mat getImage();
};

#endif // CAMERA_H