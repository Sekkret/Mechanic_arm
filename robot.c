#include "robot.h"


void setSmallServo(int deg) {
	
	deg=deg-45;
	deg = 200*deg/9 + 2000;
	if(deg>=1000 && deg<=5000)
			OCR1B=deg;
}

void setBigServo(int deg) {
	deg=90-deg;
	deg = 200*deg/9 + 2000;
	if(deg>=2000 && deg<=5000)
			OCR1A=deg;
}

void prepare(uint8_t a,  uint8_t b, float* theta, float* phi, unsigned int N) {
	for(int t=0; t<N; t++) {
		//elipsa
		float x = a * cos(2* M_PI*t/N)+a+DIST;
		float y = b * sin(2*M_PI*t/N);
		
		//uklad biegunowy
		float rho = sqrt(x*x+y*y);
		float alpha = atan(y/x);
		
		//na kąty ramienia
		phi[t] = acos(rho*rho/2/L/L - 1);
		theta[t] = phi[t]/2 - alpha;
		
		//rad do deg
		phi[t] = 180 * phi[t] / M_PI;
		theta[t] = 180 * theta[t] / M_PI;
	}
}

void draw(float* theta, float* phi, int N) {
	
	for(int i=0; i<N; i++) {
		setBigServo(theta[i]);
		setSmallServo(phi[i]);
		_delay_ms(300);
	}
	
}
