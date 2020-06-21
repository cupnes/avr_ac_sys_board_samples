/* PWMでビデオSYNCを生成しながら描画を試す */

/* Hsync期間: 3.8 us
 * Hsync周期: 64.8 us
 * Vsync期間: 512 us
 * Vsync周期: 16.5 ms */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define CNT_HSYNC	3
#define CNT_VIDEO	40

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

void init_video_sync(void)
{
	TIMSK0 = _BV(OCIE0B);

	OCR0A = CNT_HSYNC;
	OCR0B = CNT_VIDEO;

	DDRD |= _BV(PD6);

	TCCR0A = _BV(COM0A1) | _BV(COM0A0) | _BV(WGM01) | _BV(WGM00);
	TCCR0B = 0;
	TCNT0 = 0;
}

void init_video_r(void)
{
	DDRC |= _BV(PC0);
	PORTC &= ~_BV(PC0);
}

void start_video_sync(void)
{
	TCCR0B |= _BV(CS00);
}

int main(void)
{
	init_video_sync();
	init_video_r();

	sei();

	start_video_sync();

	while (1) {
		if (hsync_cntr < 248) {
			PORTC |= _BV(PC0);

			_delay_us(40);

			PORTC &= ~_BV(PC0);
		}
	}

	return 0;
}
