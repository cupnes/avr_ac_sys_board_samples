.global	main
	.type	main, @function
main:
	.L__stack_usage = 0
	sbi	0x04, 6
	sbi	0x07, 0
	sbi	0x07, 1
	sbi	0x07, 2
	out	0x08, r15
	clr	r15
	ldi	r16, 0x01
	ldi	r17, 0x02
	ldi	r18, 0x03
	ldi	r19, 0x04
	ldi	r20, 0x05
	ldi	r21, 0x06
	ldi	r22, 0x07
main_loop:
