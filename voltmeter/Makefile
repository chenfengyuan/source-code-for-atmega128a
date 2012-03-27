CC=avr-gcc
CFLAGS=-Wall -Wextra -mmcu=atmega128 -Os
AVRDUDE=avrdude -p m128 -c usbasp -e -U flash:w:
OBJCOPY=avr-objcopy -j .text -j .data -O ihex
foo:
.PHONY:max_7219_test_download
max_7219_test_download:max_7219_test.hex
	$(AVRDUDE)max_7219_test.hex

max_7219_test.hex:max_7219_test.out
	$(OBJCOPY) max_7219_test.out max_7219_test.hex

max_7219_test.out:max_7219_test.c f_cpu.h spi.o max_7219.o
	$(CC) $(CFLAGS) -o max_7219_test.out max_7219_test.c spi.o max_7219.o

max_7219.o:max_7219.c max_7219.h
	$(CC) $(CFLAGS) -c -o max_7219.o max_7219.c
spi.o:spi.c spi_pin_config.h
	$(CC) $(CFLAGS) -c -o spi.o spi.c

.PHONY:clean
clean:
	rm -fv *.out *.hex *.o
