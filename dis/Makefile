CC=avr-gcc
CFLAGS=-Wall -Wextra -mmcu=atmega128 -Os

all:a.out dis_sample.c dis_sample.o write

a.out:dis_sample.o
	$(CC) $(CFLAGS) dis_sample.o
dis_sample.o:dis_sample.c dis.h
	$(CC) $(CFLAGS) -c -o dis_sample.o dis_sample.c
a.hex:a.out
	avr-objcopy -j .text -j .data -O ihex a.out a.hex
write:a.hex
	avrdude -p m128 -c usbasp -e -U flash:w:a.hex
clean:
	rm -f a.hex *.o a.out
