CC=avr-gcc
CFLAGS=-Wall -Wextra -mmcu=atmega128 -O1 -save-temps -g
AVRDUDE=avrdude -p m128 -c usbasp -e -U flash:w:
OBJCOPY=avr-objcopy -j .text -j .data -O ihex
foo:
.PHONY:download
download:main.hex
	$(AVRDUDE)main.hex
main.hex:main.out
	$(OBJCOPY) main.out main.hex
main.out:main.c adc.o lcd12864.o lcd12864.h adc.h Makefile extern.o int.o
	$(CC) $(CFLAGS) -o main.out main.c adc.o lcd12864.o extern.o int.o
adc.o:adc.c adc.h Makefile
	$(CC) $(CFLAGS) -c -o adc.o adc.c

lcd12864.o:lcd12864.c lcd12864.h Makefile
	$(CC) $(CFLAGS) -c -o lcd12864.o lcd12864.c
extern.o:extern.h extern.c
	$(CC) $(CFLAGS) -c -o extern.o extern.c
int.o:int.c int.h extern.o
	$(CC) $(CFLAGS) -c -o int.o int.c
.PHONY:clean
clean:
	rm -fv *.out *.hex *.o
