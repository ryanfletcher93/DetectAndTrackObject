#ifndef TRACKER_H
#define TRACKER_H

#include <opencv2/opencv.hpp>

#include "Detector.h"


class Tracker {
private:


public:
	Tracker();

	virtual void updateTracking(DetectorResults* detectorResults) = 0 ;

};



class KalmanTracker : public Tracker {
private:


public:
	KalmanTracker();

	void updateTracking(DetectorResults* detectorResults);
};


#endif // TRACKER_H