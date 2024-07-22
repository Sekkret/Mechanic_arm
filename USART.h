#ifndef _USART_H
#define _USART_H
	#include <avr/io.h>
	#include <avr/interrupt.h>
	#include <stdio.h>
	
	void USART_Init(unsigned int ubrr_value);
	void USART_Transmit(unsigned char data);
	unsigned char USART_Receive(void);
	int u_putc(char c, FILE *unused);
	
#endif
