CC=avr-gcc
CFLAGS=-Wall -Wextra -mmcu=atmega128 -Os

foo:
.PHONY:write-master
write-master:master.hex
	avrdude -p m128 -c usbasp -e -U flash:w:master.hex
master.hex:master.out
	avr-objcopy -j .text -j .data -O ihex master.out master.hex
master.out:master.c spi.h
	$(CC) $(CFLAGS) -o master.out master.c
.PHONY:write-slave
write-slave:slave.hex
	avrdude -p m128 -c usbasp -e -U flash:w:slave.hex
slave.hex:slave.out
	avr-objcopy -j .text -j .data -O ihex slave.out slave.hex
slave.out:slave.c spi.h
	$(CC) $(CFLAGS) -o slave.out slave.c
.PHONY:clean
	rm master.hex master.out slave.hex slave.out