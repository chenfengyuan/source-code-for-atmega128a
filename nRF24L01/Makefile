CC=avr-gcc
CFLAGS=-Wall -Wextra -mmcu=atmega128 -Os

receive:receive.hex
	avrdude -p m128 -c usbasp -e -U flash:w:receive.hex
receive.hex:receive.out
	avr-objcopy -j .text -j .data -O ihex receive.out receive.hex
receive.out:receive.o
	$(CC) $(CFLAGS) receive.o -o receive.out
receive.o:receive.c
	$(CC) $(CFLAGS) -c -o receive.o receive.c
send:send.hex
	avrdude -p m128 -c usbasp -e -U flash:w:send.hex
send.hex:send.out
	avr-objcopy -j .text -j .data -O ihex send.out send.hex
send.out:send.o
	$(CC) $(CFLAGS) send.o -o send.out
send.o:send.c
	$(CC) $(CFLAGS) -c -o send.o send.c
clean:
	rm *.hex *.o *.out
