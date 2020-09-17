/* 20MHz駆動
 *
 * TC0(OC0AでHsync生成)
 * - OC0A: Hblank(Low生成期間)
 * - OC0B: 次のHblankまでHigh出力
 *         割り込みでTCNT0=0xffにして次の周期へ進める
 *
 * Vcycle N回に1行だけRを描画するようにする
 * -> 全何行あるのかを確認
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
unsigned char num_vcyc = 0;

struct {
	unsigned char x;
	unsigned char y;
	unsigned char w;
	unsigned char h;
} rect = {
	5, 20, 7, 20
};

unsigned char t_up = 0;
unsigned char t_down = 0;
unsigned char t_left = 0;
unsigned char t_right = 0;

ISR(TIMER0_COMPA_vect)
{
	volatile unsigned char _c = 7;
	while (_c--);

	if ((num_hcyc > rect.y) && (num_hcyc < (rect.y + rect.h))) {
		/* blank */
		_c = rect.x;
		while (_c--);

		/* draw */
		PORTC |= _BV(PC0);
		_c = rect.w;
		while (_c--);

		/* blank */
		PORTC &= ~_BV(PC0);
	}
}

#define BTN_UP_TH	3
#define BTN_DOWN_TH	3
#define BTN_LEFT_TH	6
#define BTN_RIGHT_TH	6
ISR(TIMER0_COMPB_vect)
{
	num_hcyc++;
	if (num_hcyc >= CYC_VSYNC_START) {
		OCR0A = 0xff;

		if (!(PINB & _BV(PINB1))) {
			if (++t_up > BTN_UP_TH) {
				t_up = 0;
				rect.y--;
			}
		}
		if (!(PINB & _BV(PINB2))) {
			if (++t_down > BTN_DOWN_TH) {
				t_down = 0;
				rect.y++;
			}
		}
		if (!(PIND & _BV(PIND0))) {
			if (++t_left > BTN_LEFT_TH) {
				t_left = 0;
				rect.x--;
			}
		}
		if (!(PIND & _BV(PIND1))) {
			if (++t_right > BTN_RIGHT_TH) {
				t_right = 0;
				rect.x++;
			}
		}
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

void init_button_up(void)
{
	DDRB &= ~_BV(DDB1);
	PORTB |= _BV(PORTB1);
}

void init_button_down(void)
{
	DDRB &= ~_BV(DDB2);
	PORTB |= _BV(PORTB2);
}

void init_button_left(void)
{
	DDRD &= ~_BV(DDD0);
	PORTD |= _BV(PORTD0);
}

void init_button_right(void)
{
	DDRD &= ~_BV(DDD1);
	PORTD |= _BV(PORTD1);
}

void start_video_sync(void)
{
	TCCR0B |= _BV(CS01);
}

int main(void)
{
	init_hsync();
	init_video_r();
	init_button_up();
	init_button_down();
	init_button_left();
	init_button_right();

	sei();

	start_video_sync();

	while (1);

	return 0;
}
