CC=avr-gcc
CFLAGS=-Wall -Wextra -mmcu=atmega128 -Os

write:a.hex
	avrdude -p m128 -c usbasp -e -U flash:w:a.hex
a.hex:a.out
	avr-objcopy -j .text -j .data -O ihex a.out a.hex
a.out:max_7219_sample.o
	$(CC) $(CFLAGS) max_7219_sample.o
max_7219_sample.o:max_7219_sample.c max_7219.h
	$(CC) $(CFLAGS) -c -o max_7219_sample.o max_7219_sample.c
clean:
	rm a.hex a.out max_7219_sample.o
