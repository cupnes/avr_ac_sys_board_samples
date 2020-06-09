/* ボタン1,2,3の押下に応じてR,G,BをHにする */

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

#define button_port PORTD
#define button_pin PIND
#define button_direction DDRD

#define button_1 (1 << PD0)

int main(void)
{
	output(video_sync_direction, video_sync);
	output(video_data_direction, video_data_r);
	output(video_data_direction, video_data_g);
	output(video_data_direction, video_data_b);
	input(button_direction, button_1);

	clear(video_data_port, video_data_r);
	clear(video_data_port, video_data_g);
	clear(video_data_port, video_data_b);
	set(button_port, button_1);

	while (1) {
		/* Begin: Vsync */
		clear(video_sync_port, video_sync);
		_delay_us(537);
		/* End: Vsync */

		unsigned int i;
		for (i = 0; i < 248; i++) {
			unsigned char is_button_1 = 0;

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
			if (!(button_pin & button_1)) {
				set(video_data_port, video_data_r);
				is_button_1 = 1;
			} else {
				asm("nop");
				asm("nop");
				asm("nop");
				asm("nop");
			}
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
			if (is_button_1) {
				clear(video_data_port, video_data_r);
			}
		}
	}

	return 0;
}
