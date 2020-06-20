/* PWMでビデオSYNCの生成を試す */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define CNT_HSYNC	4
#define CNT_VIDEO	67

uint8_t hsync_cntr = 0;
ISR(TIMER0_COMPB_vect)
{
	hsync_cntr++;
	if (hsync_cntr < 248) {
		TCNT0 = 0xff;
	} else {
		TCCR0B = 0;
		TCCR0A = 0;
		PORTD &= ~_BV(PD6);

		_delay_us(537);

		TCNT0 = 0;
		TCCR0A = _BV(COM0A1) | _BV(COM0A0) | _BV(WGM01) | _BV(WGM00);
		TCCR0B = _BV(CS00);

		hsync_cntr = 0;
	}
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
