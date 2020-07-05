/* 20MHz駆動
 * TC0(OC0AでHsync生成)
 * - OC0A: Hblank(Low生成期間)
 * - OC0B: 次のHblankまでHigh出力
 *         割り込みでTCNT0=0xffにして次の周期へ進める
 */

/* Hsync期間: 4 us
 * Hsync周期: 64 us
 * Vsync期間: 512 us
 * Vsync周期: 16.384 ms */

#include <avr/io.h>
#include <avr/interrupt.h>

#define CNT_HSYNC	9
#define CNT_HCYC	155
#define CYC_VSYNC_START	248

unsigned char num_hcyc = 0;

ISR(TIMER0_COMPA_vect)
{
	volatile unsigned char _c = 7;
	while (_c--);
	PORTC |= _BV(PC0);
}

ISR(TIMER0_COMPB_vect)
{
	PORTC &= ~_BV(PC0);

	num_hcyc++;
	if (num_hcyc >= 248) {
		OCR0A = 0xff;
	} else {
		OCR0A = CNT_HSYNC;
	}

	TCNT0 = 0xff;
}

void init_hsync(void)
{
	TIMSK0 = _BV(OCIE0A) | _BV(OCIE0B);

	OCR0A = CNT_HSYNC;
	OCR0B = CNT_HCYC;

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
	TCCR0B |= _BV(CS01);
}

int main(void)
{
	init_hsync();
	init_video_r();

	sei();

	start_video_sync();

	while (1);

	return 0;
}
