# 7	CKDIV8		1(unprogrammed)
# 6	CKOUT		1(unprogrammed)
# 5-4	SUT[1:0]	11
#
# 3-1	CKSEL[3:1]	011
# 0	CKSEL0		1
#
# 0xf7 (Def: 0x62)
#
# read:
# sudo avrdude -p m168p -c avrisp2 -U lfuse:r:lfuse.dat:r
# write:
# sudo avrdude -p m168p -c avrisp2 -U lfuse:w:0xf7:m

# 7	CKDIV8		1(unprogrammed)
# 6	CKOUT		0(programmed)
# 5-4	SUT[1:0]	11
#
# 3-1	CKSEL[3:1]	011
# 0	CKSEL0		1
#
# 0xb7 (Def: 0x62)
#
# read:
# sudo avrdude -p m168p -c avrisp2 -U lfuse:r:lfuse.dat:r
# write:
# sudo avrdude -p m168p -c avrisp2 -U lfuse:w:0xb7:m

TARGET=asm
SRCS=main.S
F_CPU=1000000L

all: $(TARGET).hex

ispchk:
	sudo avrdude -p m168p -c avrisp2

ispwrite: $(TARGET).hex
	sudo avrdude -p m168p -c avrisp2 -U $<

$(TARGET).hex: $(TARGET).out
	avr-objcopy -O ihex $< $@

$(TARGET).out: $(SRCS)
	avr-gcc -mmcu=atmega168p -Wall -Wextra -Os -DF_CPU=$(F_CPU) -o $@ $^

clean:
	rm -f *~ *.out *.hex *.map

.PHONY: ispchk ispwrite clean
