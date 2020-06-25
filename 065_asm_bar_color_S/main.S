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
	ldi	r23, 248
hcycle_loop:
	cbi	0x05, 6
	nop
	nop
	sbi	0x05, 6
	nop
	nop
	nop
	nop
	nop
	nop
	nop

	out	0x08, r16
	out	0x08, r17
	out	0x08, r18
	out	0x08, r19
	out	0x08, r20
	out	0x08, r21
	out	0x08, r22

	out	0x08, r16
	out	0x08, r17
	out	0x08, r18
	out	0x08, r19
	out	0x08, r20
	out	0x08, r21
	out	0x08, r22

	out	0x08, r16
	out	0x08, r17
	out	0x08, r18
	out	0x08, r19
	out	0x08, r20
	out	0x08, r21
	out	0x08, r22

	out	0x08, r16
	out	0x08, r17
	out	0x08, r18
	out	0x08, r19
	out	0x08, r20
	out	0x08, r21
	out	0x08, r22

	out	0x08, r16
	out	0x08, r17
	out	0x08, r18
	out	0x08, r19
	out	0x08, r20
	out	0x08, r21
	out	0x08, r22

	out	0x08, r16
	out	0x08, r17
	out	0x08, r18
	out	0x08, r19
	out	0x08, r20
	out	0x08, r21
	out	0x08, r22

	out	0x08, r16
	out	0x08, r17
	out	0x08, r18
	out	0x08, r19
	out	0x08, r20
	out	0x08, r21

	out	0x08, r15

	dec	r23
	breq	hcycle_loop_exit
	jmp	hcycle_loop
hcycle_loop_exit:

	nop
	nop
	cbi	0x05, 6
	ldi	r24, 178
vsync_loop:
	dec	r24
	brne	vsync_loop

	jmp	main_loop

	ldi r24,0
	ldi r25,0
	ret
