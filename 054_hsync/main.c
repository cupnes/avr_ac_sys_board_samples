/* TC0のPWMでHsyncのみ生成
 * (Vsyncは別のタイマーで生成) */

/* Hsync期間: 3.8us
 * Hsync周期: 64us
 * 正常系では「Hsync期間:4us」・「Hsync周期:64us」で
 * (/ 64 4.0)16.0
 * これだと (* 3.8 16)60.8 なので「Hsync周期:60.8us」へ
 * 調整した方が良いかも知れない */

#include <avr/io.h>
#include <avr/interrupt.h>

#define CNT_HSYNC	3
#define CNT_VIDEO	45

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
