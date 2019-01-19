#ifndef TRACKER_H
#defin TRACKER_H

#include <opencv2/opencv.hpp>


class Tracker {
private:


public:
	Tracker();

	virtual void updateObjects(Detector detector) = 0 ;

}



class KalmanTracker {
private:


public:
	KalmanTracker();

	void updateObjects(Detector detector);
}


#endif // TRACKER_H