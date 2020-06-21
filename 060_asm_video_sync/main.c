#define VIDEO_SYNC_INIT	"	sbi	0x04, 6\n"
#define VIDEO_SYNC_SET	"	sbi	0x05, 6\n"
#define VIDEO_SYNC_CLR	"	cbi	0x05, 6\n"

int main(void)
{
	asm(
		VIDEO_SYNC_INIT

		"main_loop:\n"
		"	ldi	r23, 248\n"

		"hcycle_loop:\n"
		VIDEO_SYNC_CLR
		"	nop\n"
		"	nop\n"
		VIDEO_SYNC_SET
		"	ldi	r24, 19\n"
		"video_cycle_loop:\n"
		"	dec	r24\n"
		"	brne	video_cycle_loop\n"
		"	nop\n"
		"	nop\n"

		"	dec	r23\n"
		"	brne	hcycle_loop\n"

		VIDEO_SYNC_CLR
		"	ldi	r24, 179\n"
		"vsync_loop:\n"
		"	dec	r24\n"
		"	brne	vsync_loop\n"

		"	rjmp	main_loop\n"
		);

	return 0;
}
