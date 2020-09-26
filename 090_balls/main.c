/* 20MHz駆動
 *
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

#define WAIT_START_DRAW	20
#define WAIT_DRAW_WIDTH	155

#define NUM_BALLS	2

#define BALL0_INIT_X	30
#define BALL0_INIT_Y	50
#define BALL0_WIDTH	10
#define BALL0_HEIGHT	20

#define BALL1_INIT_X	100
#define BALL1_INIT_Y	70
#define BALL1_WIDTH	10
#define BALL1_HEIGHT	20

unsigned char num_hcyc = 0;
unsigned char num_vcyc = 0;
unsigned char do_update = 0;

struct rect {
	unsigned char sx;
	unsigned char sy;
	unsigned char ex;
	unsigned char ey;
	unsigned char w;
	unsigned char h;
} balls[NUM_BALLS];

unsigned int obj_seq[CYC_VSYNC_START];

ISR(TIMER0_COMPA_vect)
{
	unsigned int oseq = obj_seq[num_hcyc];

	while (TCNT0 < WAIT_START_DRAW);

	PORTC |= _BV(PC2);

	while (oseq) {
		unsigned char obj = (oseq % 16) - 1;
		oseq /= 16;
		while (TCNT0 < balls[obj].sx);
		PORTC |= _BV(PC0) | _BV(PC1);
		while (TCNT0 < balls[obj].ex);
		PORTC &= ~(_BV(PC0) | _BV(PC1));
	}

	while (TCNT0 < WAIT_DRAW_WIDTH);

	PORTC &= ~_BV(PC2);
}

ISR(TIMER0_COMPB_vect)
{
	num_hcyc++;
	if (num_hcyc >= CYC_VSYNC_START) {
		/* Vsync: 248 <= Hcyc <= 255 */
		OCR0A = 0xff;
		if (num_hcyc == CYC_VSYNC_START) {
			do_update = 1;
		}
	} else {
		/* Draw: 0 <= Hcyc <= 247 */
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

void init_video_g(void)
{
	DDRC |= _BV(PC1);
	PORTC &= ~_BV(PC1);
}

void init_video_b(void)
{
	DDRC |= _BV(PC2);
	PORTC &= ~_BV(PC2);
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

void init_balls(void)
{
	balls[0].sx = BALL0_INIT_X;
	balls[0].sy = BALL0_INIT_Y;
	balls[0].ex = BALL0_INIT_X + BALL0_WIDTH;
	balls[0].ey = BALL0_INIT_Y + BALL0_HEIGHT;
	balls[0].w = BALL0_WIDTH;
	balls[0].h = BALL0_HEIGHT;

	balls[1].sx = BALL1_INIT_X;
	balls[1].sy = BALL1_INIT_Y;
	balls[1].ex = BALL1_INIT_X + BALL1_WIDTH;
	balls[1].ey = BALL1_INIT_Y + BALL1_HEIGHT;
	balls[1].w = BALL1_WIDTH;
	balls[1].h = BALL1_HEIGHT;
}

void init_obj_seq(void)
{
	unsigned char i;
	for (i = 0; i < CYC_VSYNC_START; i++) {
		obj_seq[i] = 0;
	}
}

int main(void)
{
	init_hsync();
	init_video_r();
	init_video_g();
	init_video_b();
	init_button_up();
	init_button_down();
	init_button_left();
	init_button_right();
	init_balls();
	init_obj_seq();

	sei();

	start_video_sync();

	while (1);

	return 0;
}
