/* PB6 */
#define VIDEO_SYNC_INIT	"	sbi	0x04, 6\n"
#define VIDEO_SYNC_SET	"	sbi	0x05, 6\n"
#define VIDEO_SYNC_CLR	"	cbi	0x05, 6\n"

/* PC0 */
#define VIDEO_R_INIT	"	sbi	0x07, 0\n"
#define VIDEO_R_SET	"	out	0x08, r16\n"
#define VIDEO_R_CLR	"	out	0x08, r15\n"

/* PC1 */
#define VIDEO_G_INIT	"	sbi	0x07, 1\n"
#define VIDEO_G_SET	"	out	0x08, r17\n"
#define VIDEO_G_CLR	"	out	0x08, r15\n"

/* PC2 */
#define VIDEO_B_INIT	"	sbi	0x07, 2\n"
#define VIDEO_B_SET	"	out	0x08, r19\n"
#define VIDEO_B_CLR	"	out	0x08, r15\n"

int main(void)
{
	asm(
		VIDEO_SYNC_INIT
		VIDEO_R_INIT
		VIDEO_G_INIT
		VIDEO_B_INIT
		VIDEO_R_CLR
		VIDEO_G_CLR
		VIDEO_B_CLR

		"	clr	r15\n"
		"	ldi	r16, 0x01\n"
		"	ldi	r17, 0x02\n"
		"	ldi	r18, 0x03\n"
		"	ldi	r19, 0x04\n"
		"	ldi	r20, 0x05\n"
		"	ldi	r21, 0x06\n"
		"	ldi	r22, 0x07\n"

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

		"	ldi	r24, 9\n"
		"video_draw_loop:\n"
		VIDEO_R_SET
		VIDEO_G_SET
		VIDEO_B_SET
		"	dec	r24\n"
		"	brne	video_draw_loop\n"
		VIDEO_B_CLR

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
