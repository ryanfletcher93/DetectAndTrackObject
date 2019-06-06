#include "Motor.h"

#include <opencv2/opencv.hpp>

#include <math.h>

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