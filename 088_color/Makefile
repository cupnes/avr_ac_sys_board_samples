TARGET=color.hex
SRCS=main.c
F_CPU=1000000L

all: $(TARGET)

ispchk:
	sudo avrdude -p m168p -c avrisp2

ispwrite: $(TARGET)
	sudo avrdude -p m168p -c avrisp2 -U $<

$(TARGET): $(TARGET:%.hex=%.out)
	avr-objcopy -O ihex $< $@

$(TARGET:%.hex=%.out): $(SRCS)
	avr-gcc -mmcu=atmega168p -Wall -Wextra -Os -DF_CPU=$(F_CPU) -o $@ $^

clean:
	rm -f *~ *.out *.hex

.PHONY: ispchk ispwrite clean
