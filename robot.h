#ifndef _robot_h
#define _robot_h
	
#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
	
	#define L 9
	#define DIST 7
	
	void setSmallServo(int deg);
	void setBigServo(int deg);
	void draw(float* theta, float* phi, int N);
	void prepare(uint8_t a,  uint8_t b, float* theta, float* phi, unsigned int N);
#endif
