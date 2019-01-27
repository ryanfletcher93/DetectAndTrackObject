#ifndef TRACKER_H
#define TRACKER_H

#include <opencv2/opencv.hpp>

#include "Detector.h"


class Tracker {
private:


public:
	Tracker();

	virtual void updateObjects(Detector* detector) = 0 ;

};



class KalmanTracker {
private:


public:
	KalmanTracker();

	void updateObjects(Detector* detector);
};


#endif // TRACKER_H