/* PWMでビデオSYNCの生成を試す */
/* ・・・のは無理な感じがしてきた */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define CNT_HSYNC	3
#define CNT_VIDEO	39

uint8_t is_video_enabled = 0;
ISR(TIMER0_COMPA_vect)
{
	is_video_enabled = 1;
}

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

		_delay_us(540);

		TCNT0 = 0;
		TCCR0A = _BV(COM0A1) | _BV(COM0A0) | _BV(WGM01) | _BV(WGM00);
		TCCR0B = _BV(CS00);

		hsync_cntr = 0;
	}
}

#define MAX_DRAW_CNT	100
int main(void)
{
	DDRC |= _BV(PC0);

	TCNT0 = 0;

	TIMSK0 = _BV(OCIE0B);

	OCR0A = CNT_HSYNC;
	OCR0B = CNT_VIDEO;

	DDRD |= _BV(PD6);

	sei();

	TCCR0A = _BV(COM0A1) | _BV(COM0A0) | _BV(WGM01) | _BV(WGM00);
	TCCR0B = _BV(CS00);

	enum DRAW_STATE {
		WAITING,
		DRAWING
	};
	enum DRAW_STATE dstate = WAITING;
	while (1) {
		uint8_t t = TCNT0;
		switch (dstate) {
		case WAITING:
			if (t >= 0) {
				dstate = DRAWING;
				PORTC |= _BV(PC0);
			}
			break;
		case DRAWING:
			if (t > (CNT_VIDEO - 8)) {
				PORTC &= ~_BV(PC0);
				dstate = WAITING;
			}
			break;
		}
	}

	return 0;
}
