#include "Motor.h"

#include <stdlib.h>
#include <unistd.h>     // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <termios.h>    // POSIX terminal control definitions
#include <iostream>
#include <string>

using std::string;

ServoMotor::ServoMotor() {
	USB = open("/dev/ttyACM0", O_RDWR| O_NOCTTY);

	usleep (2000000); 
	memset (&tty, 0, sizeof tty);

	/* Error Handling */
	if ( tcgetattr ( USB, &tty ) != 0 ) {
	   std::cout << "Error " << errno << " from tcgetattr: " << strerror(errno) << std::endl;
	}

	/* Save old tty parameters */
	tty_old = tty;

	/* Set Baud Rate */
	cfsetospeed (&tty, (speed_t)B9600);
	cfsetispeed (&tty, (speed_t)B9600);

	/* Setting other Port Stuff */
	tty.c_cflag     &=  ~PARENB;            // Make 8n1
	tty.c_cflag     &=  ~CSTOPB;
	tty.c_cflag     &=  ~CSIZE;
	tty.c_cflag     |=  CS8;

	tty.c_cflag     &=  ~CRTSCTS;           // no flow control
	tty.c_cc[VMIN]   =  1;                  // read doesn't block
	tty.c_cc[VTIME]  =  5;                  // 0.5 seconds read timeout
	tty.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines

	/* Make raw */
	cfmakeraw(&tty);

	/* Flush Port, then applies attributes */
	tcflush( USB, TCIFLUSH );
	if ( tcsetattr ( USB, TCSANOW, &tty ) != 0) {
	   std::cout << "Error " << errno << " from tcsetattr" << std::endl;
	}
}

ServoMotor::~ServoMotor() {
	//close(USB);
}


/**
 * Move motors to parameters angles, angles are in degrees
 */
void ServoMotor::moveMotorToPosition(float panAngle, float tiltAngle) {

	char angleDelimeter = ':';
	char lineDelimeter = ';';

	string panString = std::to_string((int)panAngle);
	string tiltString = std::to_string((int)tiltAngle);

	string resStr = panString + ':' + tiltString + ';';

	std::cout << resStr << std::endl;

	char *resChar = strdup(resStr.c_str());
	int n_written = write(USB, resChar, sizeof(resChar) -1);

	usleep (100000); 

	std::cout << n_written << std::endl;

}