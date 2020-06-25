#!/bin/bash

# 作ってみた、けどフラッシュに収まらない

# set -uex
set -ue

hsync_first_half() {
	cat <<EOF
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
EOF
}

draw_line() {
	local y=$1
	for i in $(seq 7); do
		cat <<EOF
	out	0x08, r16
	out	0x08, r17
	out	0x08, r18
	out	0x08, r19
	out	0x08, r20
	out	0x08, r21
	out	0x08, r22
EOF
	done
	cat <<EOF
	out	0x08, r15
EOF
}

hsync_last_half() {
	cat <<EOF
	nop
	nop
	cbi	0x05, 6
EOF
}

hcycle() {
	local y=$1
	hsync_first_half
	draw_line $y
	hsync_last_half
}

vsync() {
	cat <<EOF
	ldi	r24, 178
vsync_loop:
	dec	r24
	brne	vsync_loop
EOF
}

vcycle() {
	vsync
	for y in $(seq 0 247); do
		hcycle y
	done
}

body() {
	vcycle
}

cat head.S
body
cat foot.S
