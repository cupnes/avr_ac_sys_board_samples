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

#define video_data_1 (1 << PC0)
#define video_data_2 (1 << PC1)
#define video_data_3 (1 << PC2)
#define video_data_port PORTC
#define video_data_direction DDRC

int main(void)
{
	output(video_sync_direction, video_sync);
	output(video_data_direction, video_data_1);
	output(video_data_direction, video_data_2);
	output(video_data_direction, video_data_3);

	while (1) {
		/* Begin: Vsync */
		clear(video_sync_port, video_sync);
		_delay_us(535);
		/* End: Vsync */

		unsigned int i;
		for (i = 0; i < 248; i++) {
			/* Begin: Hsync */
			clear(video_sync_port, video_sync);
			asm("nop");
			asm("nop");
			/* End: Hsync */
			set(video_sync_port, video_sync);
			set(video_data_port, video_data_3);
			set(video_data_port, video_data_2);
			set(video_data_port, video_data_1);
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
			clear(video_data_port, video_data_3);
			clear(video_data_port, video_data_2);
			clear(video_data_port, video_data_1);
		}
	}

	return 0;
}
