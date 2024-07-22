#include "USART.h"

void USART_Init ( unsigned int ubrr_value) {
	/* Prędkość transmisji */
	UBRRH = ( unsigned char ) ( ubrr_value >>8);
	UBRRL = ( unsigned char ) ubrr_value;
	/* Format ramki : słowo =8 bitów , 2 bity stopu */
	UCSRC =(1<<URSEL)|(1 <<USBS)|(1 <<UCSZ1)|(1 <<UCSZ0 );
	/* Włączenie odbiornika i nadajnika */
	UCSRB = (1<<RXEN)|(1 <<TXEN );
	//wączenie przerwania, gdy jest przyszła dana
	UCSRB |= (1<<RXCIE);
}

void USART_Transmit ( unsigned char data ) {
	/* Czekaj , aż zwolni się bufor nadajnika */
	while ( !( UCSRA & (1<<UDRE) )) ;
	/* Umieść daną w buforze i ją wyślij */
	UDR = data;
}

unsigned char USART_Receive ( void ) {
	/* Czekaj , aż pojawi się dana do odbioru */
	while ( !( UCSRA & (1<<RXC )) ) ;
	/* Odbierz daną */
	return UDR;
}
int u_putc(char c, FILE *unused) {
	USART_Transmit(c);
	return 0;
}
