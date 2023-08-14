#define bool _Bool

#ifndef BITSET_SIZE
#define BITSET_SIZE 1024
#endif

#ifdef BIT_COMPRESS

typedef uint8_t bitfield_t;

bitfield_t bitfields[BITSET_SIZE / (sizeof(bitfield_t) * 8)];

static inline
void setbit(uint32_t bit, bool value)
{
	uint8_t s = sizeof(bitfield_t) * 8;
	uint8_t shift = bit % s;
	uint32_t off = bit / s;
	if (value)
		bitfields[off] |= (1 << shift);
	else
		bitfields[off] &= ~(1 << shift);
}

static inline
bool getbit(uint32_t bit)
{
	uint8_t s = sizeof(bitfield_t) * 8;
	uint8_t shift = bit % s;
	uint32_t off = bit / s;
	bool value = bitfields[off] & (1 << shift);
	return value;
}

#else

bool bitfields[BITSET_SIZE];

static inline
void setbit(uint32_t bit, bool value)
{
	bitfields[bit] = value;
}

static inline
bool getbit(uint32_t bit)
{
	return bitfields[bit];
}

#endif
