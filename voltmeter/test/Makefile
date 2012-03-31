CC=avr-gcc
CFLAGS=-Wall -Wextra -mmcu=atmega128 -Os -g
AVRDUDE=avrdude -p m128 -c usbasp -e -U flash:w:
OBJCOPY=avr-objcopy -j .text -j .data -O ihex
nil:
.PHONY:out_write
out_write:out.hex
	$(AVRDUDE)out.hex

out.hex:out.out
	$(OBJCOPY) out.out out.hex

out.out:out.c
	$(CC) $(CFLAGS) -o out.out out.c
.PHONY:ad_test_write
ad_test_write:ad_test.hex
	$(AVRDUDE)ad_test.hex

ad_test.hex:ad_test.out
	$(OBJCOPY) ad_test.out ad_test.hex

ad_test.out:ad_test.c ../spi.o ../max_7219.o
	$(CC) $(CFLAGS) -o ad_test.out ad_test.c ../spi.o  ../max_7219.o

.PHONY:lcd12864_write
lcd12864_write:lcd12864.hex
	$(AVRDUDE)lcd12864.hex

lcd12864.hex:lcd12864.out
	$(OBJCOPY) lcd12864.out lcd12864.hex

lcd12864.out:lcd12864.c
	$(CC) $(CFLAGS) -o lcd12864.out lcd12864.c

.PHONY:clean
clean:
	rm -fv *.out *.hex *.o *.s
