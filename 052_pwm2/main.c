/* PWMでビデオSYNCの生成を試す */

#include <avr/io.h>
#include <avr/interrupt.h>

#define CNT_HSYNC	4
#define CNT_VIDEO	67

uint8_t is_hsync = 1;
ISR(TIMER0_COMPB_vect)
{
	TCNT0 = 0xff;
}

int main(void)
{
	TCNT0 = 0;

	TIMSK0 = _BV(OCIE0B);

	OCR0A = CNT_HSYNC;
	OCR0B = CNT_VIDEO;

	DDRD |= _BV(PD6);

	sei();

	TCCR0A = _BV(COM0A1) | _BV(COM0A0) | _BV(WGM01) | _BV(WGM00);
	TCCR0B = _BV(CS00);

	while (1);

	return 0;
}
