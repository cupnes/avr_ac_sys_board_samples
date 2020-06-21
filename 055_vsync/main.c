/* TC1のPWMでVsync生成
 * (TC0のPWMでHsync生成) */

#include <avr/io.h>
#include <avr/interrupt.h>

#define CNT_HSYNC	3
#define CNT_HSYNC_CYCLE	45

#define CNT_VSYNC	7
#define CNT_VSYNC_CYCLE	268

ISR(TIMER0_COMPB_vect)
{
	TCNT0 = 0xff;
}

ISR(TIMER1_COMPB_vect)
{
	TCNT1 = 0x01ff;
}

void init_pwm_hsync(void)
{
	TIMSK0 = _BV(OCIE0B);

	OCR0A = CNT_HSYNC;
	OCR0B = CNT_HSYNC_CYCLE;

	DDRD |= _BV(PD6);

	TCCR0A = _BV(COM0A1) | _BV(COM0A0) | _BV(WGM01) | _BV(WGM00);
	TCCR0B = 0;
	TCNT0 = 0;
}

/*
1周期: 62.4ms
カウント幅: 10bit(MAX:1024)
(/ (* 62.4 1000) 1024.0)60.9375us/clk

(/ (* 64 256) 60.9375)268.865641025641 clk
268 = 0x10C

(/ 512 60.9375)8.402051282051282 clk
 */
void init_pwm_vsync(void)
{
	TIMSK1 = _BV(OCIE1B);

	OCR1A = CNT_VSYNC;
	OCR1B = CNT_VSYNC_CYCLE;

	DDRB |= _BV(PB1);

	TCCR1A = _BV(COM1A1) | _BV(COM1A0) | _BV(WGM11);
	TCCR1B = _BV(WGM12);
	TCNT1 = 0;
}

void start_pwm_hvsync(void)
{
	TCCR0B |= _BV(CS00);
	TCCR1B |= _BV(CS11) | _BV(CS10);
}

int main(void)
{
	init_pwm_hsync();
	init_pwm_vsync();

	sei();

	start_pwm_hvsync();

	while (1);

	return 0;
}
