#ifndef ACTION_END_EFFECTOR_H
#define ACTION_END_EFFECTOR_H


class ActionEndEffector {
private:


public:
	ActionEndEffector();

	virtual void doPrimaryAction() = 0;
};



class ArduinoFire : ActionEndEffector {
private:


public:
	ArduinoFire();

	void doPrimaryAction();
};


#endif //ACTION_END_EFFECTOR_H