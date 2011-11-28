CC=avr-gcc
CFLAGS=-Wall -Wextra -mmcu=atmega128 -Os

.PHONY:write
write:a.hex
	avrdude -p m128 -c usbasp -e -U flash:w:a.hex
a.hex:a.out
	avr-objcopy -j .text -j .data -O ihex a.out a.hex
a.out:ds1302_sample.o
	$(CC) $(CFLAGS) ds1302_sample.o
max_7219_sample.o:ds1302_sample.c ds1302.h ./max_7219/max_7219.h
	$(CC) $(CFLAGS) -c -o ds1302_sample.o ds1302_sample.c
.PHONY: clean
clean:
	rm a.hex a.out ds1302_sample.o
