#ifndef CAMERA_H
#define CAMERA_H

#include <opencv2/opencv.hpp>

class Camera {
	int a;

public:
	Camera();

	virtual cv::Mat getImage() = 0;
};

class IpCamera {
	std::string ipAddr;

public:
	IpCamera();

	std::string getIpAddr() {
		return this->ipAddr;
	}
	void setIpAddr(std::string ipAddr) {
		this->ipAddr = ipAddr;
	}

	cv::Mat getImage();
};

#endif // CAMERA_H