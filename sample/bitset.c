#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define OUT ((void *) 0x08100000)
long out_offset = 0;

#define BITSET_SIZE (1280)
#include "bitset.h"

void *memset32(void* s, uint32_t b, size_t n) {
	uint32_t *p = (uint32_t*) s;
	for (int i = 0; i < n; i ++) {
		p[i] = b;
	}
	return s;
}

void *memset16(void* s, uint16_t b, size_t n) {
	uint16_t *p = (uint16_t*) s;
	for (int i = 0; i < n; i ++) {
		p[i] = b;
	}
	return s;
}

void *memset8(void* s, uint8_t b, size_t n) {
	uint8_t *p = (uint8_t*) s;
	for (int i = 0; i < n; i ++) {
		p[i] = b;
	}
	return s;
}

void *memset(void* s, int c, size_t n) {
	uint32_t b = 0;
	switch (n % 4) {
		case 0:
			// 32b aligned
			b |= c << 0;
			b |= c << 8;
			b |= c << 16;
			b |= c << 24;
			return memset32(s, c, n / 4);
		case 2:
			// 16b aligned
			b |= c << 0;
			b |= c << 8;
			return memset32(s, c, n / 2);
		default:
			// 8b aligned
			return memset8(s, c, n);

	}
}

void *memcpy(void* dst, const void *src, size_t len)
{
	uint32_t* dst32 = (uint32_t*) dst;
	uint32_t* src32 = (uint32_t*) src;

	int is_dst_aligned = ((int) dst32 % 4) == 0;
	int is_src_aligned = ((int) src32 % 4) == 0;
	if (is_dst_aligned && is_src_aligned) {
		while (len > 3) { 
			*dst32++ = *src32++;
			len -= 4;
		}

	}

	uint8_t *dst8 = (uint8_t*) dst32;
	uint8_t *src8 = (uint8_t*) src32;

	while (len--) {
		*dst8++ = *src8++;
	}

	return (dst);
}

__attribute__((weak))
void main() {
	for (int i = 0; i < BITSET_SIZE; i ++) {
		setbit(i, i % 3);
	}
}
