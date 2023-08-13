#define bool _Bool

#ifndef BITSET_SIZE
#define BITSET_SIZE 1024
#endif

#ifdef BIT_COMPRESS

uint32_t bitfields[BITSET_SIZE / (sizeof(uint32_t) * 8)];

void setbit(uint32_t bit, bool value)
{
	uint8_t s = sizeof(uint32_t) * 8;
	uint8_t shift = bit % s;
	uint32_t off = bit / s;
	if (value)
		bitfields[off] |= (1 << shift);
	else
		bitfields[off] &= ~(1 << shift);
}

bool getbit(uint32_t bit)
{
	uint8_t s = sizeof(uint32_t) * 8;
	uint8_t shift = bit % s;
	uint32_t off = bit / s;
	bool value = bitfields[off] & (1 << shift);
	return value;
}

#else

bool bitfields[BITSET_SIZE];

void setbit(uint32_t bit, bool value)
{
	bitfields[bit] = value;
}

bool getbit(uint32_t bit)
{
	return bitfields[bit];
}

#endif
