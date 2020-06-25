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
