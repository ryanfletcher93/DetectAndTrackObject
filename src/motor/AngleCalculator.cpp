#include "Motor.h"

#include <opencv2/opencv.hpp>

#include <math.h>


cv::Point3f AngleCalculator::transformCoord(cv::Point3f coord, float translation[], float rotationAngles[]) {

	cv::Point3f resultCoord;

	cv::Point3f transCoord = coord;
	transCoord.x += translation[0];
	transCoord.y += translation[1];
	transCoord.z += translation[2];

	float cosa = cos(rotationAngles[0]);
	float sina = sin(rotationAngles[0]);

	float cosb = cos(rotationAngles[1]);
	float sinb = sin(rotationAngles[1]);

	float cosc = cos(rotationAngles[2]);
	float sinc = sin(rotationAngles[2]);

	float Axx = cosa*cosb;
    float Axy = cosa*sinb*sinc - sina*cosc;
    float Axz = cosa*sinb*cosc + sina*sinc;

    float Ayx = sina*cosb;
    float Ayy = sina*sinb*sinc + cosa*cosc;
    float Ayz = sina*sinb*cosc - cosa*sinc;

    float Azx = -sinb;
    float Azy = cosb*sinc;
    float Azz = cosb*cosc;

    resultCoord.x = Axx*transCoord.x + Axy*transCoord.y + Axz*transCoord.z;
    resultCoord.y = Ayx*transCoord.x + Ayy*transCoord.y + Ayz*transCoord.z;
    resultCoord.z = Azx*transCoord.x + Azy*transCoord.y + Azz*transCoord.z;

    return resultCoord;
}


float AngleCalculator::getPanAngle(cv::Point3f point, int offset) {
	float noOffsetAngle = atan(point.x / point.y);
	float extraAngleFromOffset = asin(offset / hypot((float) point.x, (float) point.y));

	float totalAngle = noOffsetAngle + extraAngleFromOffset;
	return totalAngle;
}


float AngleCalculator::getTiltAngle(cv::Point3f point, int armOffset, int zCoordOffset) {
	float adjustedZCoord = point.z - zCoordOffset;

	float noArmOffsetAngle = atan(adjustedZCoord / (float)point.x);
	float extraAngleFromOffset = asin((float)armOffset / hypot((float)adjustedZCoord, (float)point.x));

	float totalAngle = noArmOffsetAngle + extraAngleFromOffset;
	return totalAngle;
}