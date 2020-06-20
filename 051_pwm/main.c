/* PWMでビデオSYNCの生成を試す */

#include <avr/io.h>
#include <avr/interrupt.h>

int main(void)
{
	TCNT0 = 0x00;
	OCR0A = 0xfe;

	DDRD |= _BV(PD6);

	TCCR0A= _BV(COM0A1) | _BV(WGM01) | _BV(WGM00);
	TCCR0B= _BV(CS00);

	while (1);

	return 0;
}
