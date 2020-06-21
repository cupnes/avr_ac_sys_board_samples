#define VIDEO_SYNC_INIT	"	sbi	0x04, 6\n"
#define VIDEO_SYNC_SET	"	sbi	0x05, 6\n"
#define VIDEO_SYNC_CLR	"	cbi	0x05, 6\n"

#define VIDEO_R_INIT	"	sbi	0x07, 0\n"
#define VIDEO_R_SET	"	sbi	0x08, 0\n"
#define VIDEO_R_CLR	"	cbi	0x08, 0\n"

int main(void)
{
	asm(
		VIDEO_SYNC_INIT
		VIDEO_R_INIT
		VIDEO_R_CLR

		"main_loop:\n"
		"	ldi	r23, 248\n"

		"hcycle_loop:\n"
		VIDEO_SYNC_CLR
		"	nop\n"
		"	nop\n"
		VIDEO_SYNC_SET
		"	nop\n"
		"	nop\n"
		"	nop\n"
		"	nop\n"

		VIDEO_R_SET
		"	ldi	r24, 17\n"
		"video_r_fill_loop:\n"
		"	dec	r24\n"
		"	brne	video_r_fill_loop\n"
		VIDEO_R_CLR

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
