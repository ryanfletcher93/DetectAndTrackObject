#ifndef CAMERA_H
#define CAMERA_H

#include <opencv2/opencv.hpp>

/**
 *
 */
class Camera {
private:

protected:
	cv::VideoCapture video;
	std::string cameraIntrinsicsFileName;

	cv::Mat K, D;
	int boardWidth, boardHeight;
	float squareSize;

	virtual bool openCamera() = 0;
	virtual bool closeCamera() = 0;
	virtual cv::Mat getImage() = 0;

public:
	virtual cv::Mat getSingleImage();
	virtual cv::Mat getImageOnButtonPress();
	virtual std::vector<cv::Mat> getImagesOnButtonPress(int numImages);
	virtual std::vector<cv::Mat> getImagesUsingDelay(int numImages, int delay);
	virtual std::vector<cv::Mat> getImagesUntilCharacter(char stopChar);

	virtual void getCameraIntrinsicsFromFile(std::string filePath);
};

/**
 *
 */
class IpCamera : public Camera {
private:
	std::string ipAddr;

protected:
	bool openCamera();
	bool closeCamera();
	cv::Mat getImage();

public:
	IpCamera(std::string ipAddr, std::string intrinsicsFilePath);

	std::string getIpAddr() {
		return this->ipAddr;
	}
	void setIpAddr(std::string ipAddr) {
		this->ipAddr = ipAddr;
	}
};

/**
 *
 */
class VideoFromFile : public Camera {
private:
	std::string fileName;

protected:
	bool openCamera();
	bool closeCamera();
	cv::Mat getImage();

public:
	VideoFromFile(std::string videoFilePath, std::string intrinsicsFilePath);

	std::string getFileName() {
		return this->fileName;
	}
	void setFileName(std::string fileName) {
		this->fileName = fileName;
	}
};

#endif // CAMERA_H