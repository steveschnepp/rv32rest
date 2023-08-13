#ifndef ASSERT_H_
#define ASSERT_H_

inline
static
void _assert(int i) {
#ifdef DEBUG
	char *c = 0;
	if (!i) *c = ' ';
#endif
}

#endif
