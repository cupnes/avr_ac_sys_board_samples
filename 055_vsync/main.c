/* TC1のPWMでVsync生成
 * (TC0のPWMでHsync生成) */

#include <avr/io.h>
#include <avr/interrupt.h>

#define CNT_HSYNC	3
#define CNT_VIDEO	45

ISR(TIMER0_COMPB_vect)
{
	TCNT0 = 0xff;
}

void init_pwm_hsync(void)
{
	TIMSK0 = _BV(OCIE0B);

	OCR0A = CNT_HSYNC;
	OCR0B = CNT_VIDEO;

	DDRD |= _BV(PD6);

	TCCR0A = _BV(COM0A1) | _BV(COM0A0) | _BV(WGM01) | _BV(WGM00);
	TCCR0B = 0;
	TCNT0 = 0;
}

void start_pwm_hvsync(void)
{
	TCCR0B = _BV(CS00);
}

int main(void)
{
	init_pwm_hsync();
	/* init_pvm_vsync(); */

	sei();

	start_pwm_hvsync();

	while (1);

	return 0;
}
