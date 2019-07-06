#ifndef MOTOR_H
#define MOTOR_H

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

#include <termios.h>

class Motor {
protected:
	int USB;

	struct termios tty;
	struct termios tty_old;

public:
	Motor();

	virtual void moveMotorToPosition(float panAngle, float tiltAngle) = 0;
};


class ServoMotor : public Motor {
private:

public:
	ServoMotor();
	~ServoMotor();

	void moveMotorToPosition(float panAngle, float tiltAngle);
};

#endif //MOTOR_H


class AngleCalculator {
private:
	float gravity;
	float airResistance;

public:
	AngleCalculator(float g = 0.0, float airResistance = 0.0) : gravity(g), airResistance(airResistance) {}

	void setGravity(float g) {gravity = g;}

	/**
	 * Get angle from when the motor is parallel to front axis to where the
	 * end effector is aligned with the point on the pan axis. Offset is the
	 * distance in mm when looking down on pan tilt mechanism. Reference coordinates
	 * are assumed to be on pan axis

		----     ^
		|  |     | Forwards
		----

		--> Positive offset

	 *
	 * @param point is the 3D point
	 * @param offset is the perpendicular offset to front axis in mm
	 *
	 * @return the angle in degrees
	 */
	float getPanAngle(cv::Point3f point, int offset);

	/**
	 * Get angle from when the motor is parallel to floor to where end effector
	 * is aligned with point on tilt axis. 
	 *
	 * @param point is the 3D point
	 * @param armOffset is the fixed offset from the shaft that the end effector sits
	 * @param redCoordZOffset is the Z offset from the reference coords in mm
	 *
	 * @return the angle in degrees
	 */
	float getTiltAngle(cv::Point3f point, int armOffset, int refCoordZOffset);
};