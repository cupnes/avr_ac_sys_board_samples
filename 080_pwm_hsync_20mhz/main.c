/* 20MHz駆動
 * TC0(OC0AでHsync生成)
 * - OC0A: Hblank(Low生成期間)
 * - OC0B: 次のHblankまでHigh出力
 *         割り込みでTCNT0=0xffにして次の周期へ進める
 * TC2(OC2AでVsync生成)
 * - OC2A: Vblank(Low生成期間)
 * - OC2B: 次のVblankまでHigh出力
 *         割り込みでTCNT2=0xffにして次の周期へ進める
 * JammaビデオSYNCはANDロジックICで合成して生成
 * (なのでOC0AとOC2Aの生成はタイミングを調整) */

/* Hsync期間: 4 us
 * Hsync周期: 64 us
 * Vsync期間: 512 us
 * Vsync周期: 16.384 ms */

#include <avr/io.h>
#include <avr/interrupt.h>

#define CNT_HSYNC	9
#define CNT_HCYC	156

ISR(TIMER0_COMPB_vect)
{
	TCNT0 = 0xff;
}

void init_video_sync(void)
{
	TIMSK0 = _BV(OCIE0B);

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
	init_video_sync();
	/* init_video_r(); */

	sei();

	start_video_sync();

	while (1);

	return 0;
}
