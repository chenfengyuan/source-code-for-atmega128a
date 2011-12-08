CC=avr-gcc
CFLAGS=-Wall -Wextra -mmcu=atmega128 -Os

write:a.hex
	avrdude -p m128 -c usbasp -e -U flash:w:a.hex
a.hex:a.out
	avr-objcopy -j .text -j .data -O ihex a.out a.hex
a.out:main.o
	$(CC) $(CFLAGS) main.o
main.o:main.c
	$(CC) $(CFLAGS) -c -o main.o main.c
clean:
	rm a.hex a.out main.o
