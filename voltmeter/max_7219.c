#include "max_7219.h"

static uint8_t max_7219_decode = 0;
static char max_7219_shift_count = 0;
void (*trans) (uint16_t data);

/* basis */

/* static void  max_7219_noop(void) */
/* { */
/* 	trans(0x0); */
/* } */

static void max_7219_decode_mode(uint8_t d)
{
	trans(0x9 << 8 | d);
}

static void max_7219_shutdown(uint8_t d)
{
	trans(0xc << 8 | d);
}

static void max_7219_test(uint8_t d)
{
	trans(0xf << 8 | d);
}

/* extended */
void max_7219_scan_limit(uint8_t d)
{
	trans(0xb << 8 | d);
}

void max_7219_intensity(uint8_t d)
{
	trans(0xa << 8 | d);
}

void max_7219_dn(char d, char n, char dot)
{

	trans((n + 1) << 8 | dot << 7 | d);
}

void max_7219_test_on(void)
{
	max_7219_test(1);
}

void max_7219_test_off(void)
{
	max_7219_test(0);
}

void max_7219_decode_all_on(void)
{
	max_7219_decode_mode((1 << 8) - 1);
}

void max_7219_decode_all_off(void)
{
	max_7219_decode_mode(0);
}

void max_7219_decode_on(uint8_t n)
{
	SET(max_7219_decode, n);
	max_7219_decode_mode(max_7219_decode);
}

void max_7219_decode_off(uint8_t n)
{
	CLR(max_7219_decode, n);
	max_7219_decode_mode(max_7219_decode);
}

void max_7219_on(void)
{
	max_7219_shutdown(1);
}

void max_7219_off(void)
{
	max_7219_shutdown(0);
}

void max_7219_clear(void)
{
	int i;
	max_7219_decode_mode(0);
	for (i = 0; i < 8; ++i) {
		max_7219_dn(0, i, 0);
	}
}

int max_7219_number(int32_t d)
{
	char a[8] = { 0 };
	int i;
	char flag;
	char n = 0;
	if (d < 0) {
		flag = 1;
		d = -d;
	} else {
		flag = 0;
	}
	for (i = 0; i < 8 && d; ++i) {
		a[i] = (d % 10);
		d /= 10;
		++n;
	}
	n = n + flag;
	if (n > 8)
		return 1;
	if (flag)
		a[n - 1] = 0xa;
	max_7219_scan_limit(n - 1);
	/* max_7219_decode_mode((1<<n)-1); */
	for (i = 0; i < n; ++i) {
		max_7219_dn(a[i], i, 0);
	}
	return 0;
}

int max_7219_hex(uint8_t d, uint8_t n)
{
	char c;
	switch (d) {
	case 0:
		c = 0x7e;
		break;
	case 1:
		c = 0x30;
		break;
	case 2:
		c = 0x6d;
		break;
	case 3:
		c = 0x79;
		break;
	case 4:
		c = 0x33;
		break;
	case 5:
		c = 0x5b;
		break;
	case 6:
		c = 0x5f;
		break;
	case 7:
		c = 0x70;
		break;
	case 8:
		c = 0x7f;
		break;
	case 9:
		c = 0x7b;
		break;
	case 10:
		c = 0x77;
		break;
	case 11:
		c = 0x1f;
		break;
	case 12:
		c = 0x4e;
		break;
	case 13:
		c = 0x3d;
		break;
	case 14:
		c = 0x4f;
		break;
	case 15:
		c = 0x47;
		break;
	default:
		return 1;
		break;
	}
	max_7219_dn(c, n, 0);
	return 0;
}

int max_7219_hex_number(int32_t d)
{
	char a[8] = { 0 };
	int i;
	char flag;
	char n;
	if (d < 0) {
		n = flag = 1;
		d = -d;
	} else {
		flag = n = 0;
	}
	for (i = 0; i < 8 && d; ++i) {
		a[i] = (d % 16);
		d /= 16;
		++n;
	}
	if (n > 8)
		return 1;
	max_7219_scan_limit(n - 1);
	if (flag) {
		max_7219_dn(1, n - 1, 0);
		for (i = 0; i < n - 1; ++i) {
			max_7219_hex(a[i], i);
		}
	} else {
		for (i = 0; i < n; ++i) {
			max_7219_hex(a[i], i);
		}
	}
	return 0;
}

void max_7219_clear_shift_count(void)
{
	max_7219_shift_count = 0;
}

void max_7219_hex_and_shift_left(uint8_t d)
{
	static uint32_t num = 0x0;
	uint32_t temp;
	char i;
	temp = num = (num << 4) | d;

	if (max_7219_shift_count < 8)
		++max_7219_shift_count;
	max_7219_scan_limit(max_7219_shift_count - 1);
	for (i = 0; i < max_7219_shift_count; ++i) {
		max_7219_hex(temp & 0xf, i);
		temp >>= 4;
	}
}
