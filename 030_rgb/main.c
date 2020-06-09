#include <avr/io.h>
#include <util/delay.h>

#define output(directions,pin) (directions |= pin) // set port direction for output
#define input(directions,pin) (directions &= (~pin)) // set port direction for input
#define set(port,pin) (port |= pin) // set port pin
#define clear(port,pin) (port &= (~pin)) // clear port pin
#define pin_test(pins,pin) (pins & pin) // test for port pin
#define bit_test(byte,bit) (byte & (1 << bit)) // test for bit set

#define video_sync (1 << PB6)
#define video_sync_port PORTB
#define video_sync_direction DDRB

#define video_data_r (1 << PC0)
#define video_data_g (1 << PC1)
#define video_data_b (1 << PC2)
#define video_data_port PORTC
#define video_data_direction DDRC

#define COUNT_MAX	90

int main(void)
{
	output(video_sync_direction, video_sync);
	output(video_data_direction, video_data_r);
	output(video_data_direction, video_data_g);
	output(video_data_direction, video_data_b);

	unsigned char c = 0;
	unsigned char cur_pin = video_data_r;
	while (1) {
		/* Begin: Vsync */
		clear(video_sync_port, video_sync);
		_delay_us(527);
		if (c < COUNT_MAX) {
			c++;
		} else {
			switch (cur_pin) {
			case video_data_r:
				cur_pin = video_data_g;
				break;
			case video_data_g:
				cur_pin = video_data_b;
				break;
			case video_data_b:
				cur_pin = video_data_r;
				break;
			}
			c = 0;
		}
		/* End: Vsync */

		unsigned int i;
		for (i = 0; i < 248; i++) {
			/* Begin: Hsync */
			clear(video_sync_port, video_sync);
			asm("nop");
			asm("nop");
			/* End: Hsync */
			set(video_sync_port, video_sync);
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			set(video_data_port, cur_pin);
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			clear(video_data_port, cur_pin);
		}
	}

	return 0;
}
