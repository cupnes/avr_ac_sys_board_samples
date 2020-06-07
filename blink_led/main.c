// Blinks LED connected to Pin PA6
#include <avr/io.h>
#include <util/delay.h>

#define output(directions,pin) (directions |= pin) // set port direction for output
#define input(directions,pin) (directions &= (~pin)) // set port direction for input
#define set(port,pin) (port |= pin) // set port pin
#define clear(port,pin) (port &= (~pin)) // clear port pin
#define pin_test(pins,pin) (pins & pin) // test for port pin
#define bit_test(byte,bit) (byte & (1 << bit)) // test for bit set

#define led (1 << PB6)
#define led_port PORTB
#define led_direction DDRB

int main(void)
{
	output(led_direction, led);
	while (1) {
		set(led_port, led);
		/* _delay_us(1); */
		_delay_ms(1);
		clear(led_port, led);
		/* _delay_us(1); */
		_delay_ms(1);
	}

	return 0;
}
