#ifndef MOTOR_H
#define MOTOR_H

class Motor {
private:


public:
	Motor();

	virtual void moveMotorToPosition(int angle) = 0;
};


class ServoMotor : Motor {
private:

public:
	ServoMotor();

	void moveMotorToPosition(int angle);
};

#endif //MOTOR_H