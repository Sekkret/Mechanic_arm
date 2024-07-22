#include <avr/io.h>
#include <util/delay.h>
#include "USART.h"
#include <string.h>
#include "lcd.h"
#include "robot.h"


FILE lcd_stream = FDEV_SETUP_STREAM(lcd_putc, NULL, _FDEV_SETUP_WRITE);

FILE u_stream = FDEV_SETUP_STREAM(u_putc, NULL, _FDEV_SETUP_WRITE);
int i = 0;
volatile int deg;
uint8_t bufor[12];


//parametry elipsy

volatile uint8_t a = 2; //cm
volatile uint8_t b = 3; //cm

volatile int run = 0;

ISR(USART_RXC_vect) {
	bufor[i] = UDR;
	
	
	
	if(bufor[i] == '\r') {
		
		char command[20];
		char *element;
		
		strcpy(command, bufor); //copying bufor
		element = strtok(command, " "); //taking first word
		
		if( !strncmp( element, "SETA", 4)) {
			
			element = strtok(NULL, " "); //taking second word
			a = atoi((const char*) element);
		}
		else if(!strncmp( element, "SETB", 4)) {
			
			element = strtok(NULL, " "); //taking second word
			b = atoi((const char*) element);
		}
		else if(!strncmp( element, "MOVE1", 5)) {
			element = strtok(NULL, " "); //taking second word
			deg = atoi((const char*) element);
			setBigServo(deg);
		}
		else if(!strncmp( element, "MOVE2", 5)) {
			element = strtok(NULL, " "); //taking second word
			deg = atoi((const char*) element);
			fprintf(&u_stream, "%d", deg);
			setSmallServo(deg);
			fprintf(&u_stream, "%d", 100*deg/9 + 2000);
		}
		else if(!strncmp( element, "RUN", 3)) {
			run=1;
		}
		else {
			fprintf(&u_stream, "Niepoprawne polecenie.");
		}
		
		lcd_set_xy(0,0);
		fprintf(&lcd_stream, "a=%02d b=%02d       ", a, b);
		i=0;
		
	}
	else {
		i++;
	}
	
}

int main(void) {
	
	USART_Init(1);
	
	lcdinit();
	lcd_set_cursor(0);
	lcd_set_xy(0,0);
	fprintf(&lcd_stream, "a=%02d b=%02d       ", a, b);
	
	
	
	//konfigurujemy Fast PWM
	ICR1 = 39999; //częstotliwość PWM
	TCCR1A |= (1<<COM1A1); //compare output mode big servo
	TCCR1A |= (1<<COM1B1); //compare output mode small servo
	//ustawienie trybu Fast PWM:
	TCCR1A |= (1<<WGM11);
	TCCR1B |= (1<<WGM12);
	TCCR1B |= (1<<WGM13);
	//.
	TCCR1B |= (1<<CS11); //preskaler
	DDRD = 0xff; //making PD5 and DDR4 (OC1A and OC1B) an output
	
	
	setBigServo(1);
	setSmallServo(1);
	
	/* OCR1A=3000; //set servo at the midde
	OCR1B=2000;
	_delay_ms(2000);
	
	OCR1A=2001;
	OCR1B = 4000;
	_delay_ms(2000); */
		
	sei();
	
	while(1) {
		if(run==1) {
			lcd_set_xy(0,1);
			fprintf(&lcd_stream, "Working...    ");
			int N=100;
			float theta[N], phi[N];
			prepare(a, b, theta,  phi, N);
			draw(theta, phi, N);
			
			for(int i=0; i<N; i++) {
				fprintf(&u_stream, "%d\t%.2f\t%.2f\n", i, theta[i], phi[i]);
			}
			
			lcd_set_xy(0,1);
			fprintf(&lcd_stream, "Finished!      ");
			run=0;
		}
		
	}
	return 0;
}
