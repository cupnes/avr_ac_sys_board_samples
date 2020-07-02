.global	main
	.type	main, @function
main:
	.L__stack_usage = 0
	sbi	0x04, 1

main_loop:
	sbi	0x05, 1
	cbi	0x05, 1

	rjmp	main_loop

	ldi r24,0
	ldi r25,0
	ret
