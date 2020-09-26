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
#define WAIT_DRAW_WIDTH	150

#define SCREEN_SX	(WAIT_START_DRAW + 5)
#define SCREEN_EX	(WAIT_DRAW_WIDTH)
#define SCREEN_SY	(17)
#define SCREEN_EY	(CYC_VSYNC_START - 6)

#define NUM_BALLS	2

#define BALL0_INIT_X	30
#define BALL0_INIT_Y	70
#define BALL0_WIDTH	10
#define BALL0_HEIGHT	20

#define BALL1_INIT_X	65
#define BALL1_INIT_Y	220
#define BALL1_WIDTH	50
#define BALL1_HEIGHT	10

unsigned char num_hcyc = 0;
unsigned char num_vcyc = 0;

struct rect {
	unsigned char sx;
	unsigned char sy;
	unsigned char ex;
	unsigned char ey;
	unsigned char w;
	unsigned char h;
} balls[NUM_BALLS];

struct velocity {
	char x;
	char y;
} bv[NUM_BALLS];

unsigned int obj_seq[CYC_VSYNC_START];

ISR(TIMER0_COMPA_vect)
{
	unsigned int oseq = obj_seq[num_hcyc];

	while (TCNT0 < WAIT_START_DRAW);

	PORTC |= _BV(PC2);

	while (oseq) {
		unsigned char obj = (oseq & 0x0000000f) - 1;
		oseq >>= 4;
		while (TCNT0 < balls[obj].sx);
		PORTC |= _BV(PC0) | _BV(PC1);
		while (TCNT0 <= balls[obj].ex);
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

	bv[0].x = bv[0].y = 1;
	bv[1].x = bv[1].y = 1;
}

void init_obj_seq(void)
{
	unsigned char i;
	for (i = 0; i < CYC_VSYNC_START; i++) {
		obj_seq[i] = 0;
	}
}

void sort_candidates(unsigned char candidates[NUM_BALLS], unsigned char n)
{
	unsigned char i;
	for (i = 0; i < (n - 1); i++) {
		unsigned char j;
		for (j = 0; j < (n - i - 1); j++) {
			unsigned char obj_a = candidates[j] - 1;
			unsigned char obj_b = candidates[j + 1] - 1;
			if (balls[obj_a].sx > balls[obj_b].sx) {
				unsigned char t = candidates[j];
				candidates[j] = candidates[j + 1];
				candidates[j + 1] = t;
			}
		}
	}
}

void render(void)
{
	unsigned char y;
	for (y = 0; y < CYC_VSYNC_START; y++) {
		unsigned char candidates[NUM_BALLS];
		unsigned char i;
		unsigned char n = 0;
		for (i = 0; i < NUM_BALLS; i++) {
			if ((balls[i].sy <= y) && (y <= balls[i].ey)) {
				candidates[n++] = i + 1;
			}
		}
		if (n == 0) {
			obj_seq[y] = 0;
		} else if (n == 1) {
			obj_seq[y] = candidates[0];
		} else {
			sort_candidates(candidates, n);
			obj_seq[y] = 0;
			for (i = 0; i < n; i++) {
				obj_seq[y] += candidates[i] << (4 * i);
			}
		}
	}
}

void move_ball(unsigned char obj)
{
	balls[obj].sx += bv[obj].x;
	balls[obj].ex += bv[obj].x;
	balls[obj].sy += bv[obj].y;
	balls[obj].ey += bv[obj].y;

	unsigned char is_updated = 0;
	unsigned char i;

	for (i = 0; i < NUM_BALLS; i++) {
		if (i == obj) continue;
		if ((balls[obj].ey < balls[i].sy) || (balls[obj].sy > balls[i].ey)) continue;

		if ((balls[obj].sx > balls[i].sx) && (balls[obj].sx <= balls[i].ex)) {
			bv[obj].x = 1;
			is_updated = 1;
			break;
		} else if ((balls[obj].sx < balls[i].sx) && (balls[obj].ex >= balls[i].sx)) {
			bv[obj].x = -1;
			is_updated = 1;
			break;
		}
	}
	if (!is_updated) {
		if (balls[obj].sx <= SCREEN_SX) {
			bv[obj].x = 1;
		} else if (balls[obj].ex >= SCREEN_EX) {
			bv[obj].x = -1;
		}
	}

	is_updated = 0;
	for (i = 0; i < NUM_BALLS; i++) {
		if (i == obj) continue;
		if ((balls[obj].ex < balls[i].sx) || (balls[obj].sx > balls[i].ex)) continue;

		if ((balls[obj].sy > balls[i].sy) && (balls[obj].sy <= balls[i].ey)) {
			bv[obj].y = 1;
			is_updated = 1;
			break;
		} else if ((balls[obj].sy < balls[i].sy) && (balls[obj].ey >= balls[i].sy)) {
			bv[obj].y = -1;
			is_updated = 1;
			break;
		}
	}
	if (!is_updated) {
		if (balls[obj].sy <= SCREEN_SY) {
			bv[obj].y = 1;
		} else if (balls[obj].ey >= SCREEN_EY) {
			bv[obj].y = -1;
		}
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

	while (1) {
		move_ball(0);

		render();

		volatile unsigned int c = 1000;
		while (c--);
	}

	return 0;
}
