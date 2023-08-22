/* Simple Forth */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "printf.h"

typedef unsigned short opcode_t;
typedef unsigned short data_t;
typedef unsigned short data_idx_t;

data_idx_t data_idx;
data_t data_stack[256];

void push(data_t d) {
	data_stack[data_idx] = d;
	data_idx ++;
}

data_t peek() {
	data_t d = data_stack[data_idx];
	return d;
}

data_t pop() {
	data_t d = peek();
	data_idx --;
	return d;
}


enum opcode {
	OPCODE_DUP, OPCODE_DROP, OPCODE_SWAP, OPCODE_ROT, OPCODE_PICK, OPCODE_DEPTH,

	OPCODE_ADD, OPCODE_SUB, OPCODE_MUL, OPCODE_DIV, OPCODE_MOD, OPCODE_NEG,

	OPCODE_NOT, OPCODE_AND, OPCODE_OR, OPCODE_XOR, OPCODE_LOGIC_AND, OPCODE_LOGIC_OR, OPCODE_LOGIC_XOR, OPCODE_SHIFT_L, OPCODE_SHIFT_R,

	OPCODE_PRINT, OPCODE_EMIT, OPCODE_KEY, OPCODE_PRINT_STACK,

	OPCODE_COUNT
};

#define _0(s) s "\0"
static const char opcode_names[] =
// Stack operations
	_0("dup") _0("drop") _0("swap") _0("rot") _0("pick") _0("depth")

// Arithmetic
	_0("+") _0("-") _0("*") _0("/") _0("%")_0("neg")

// Bitwise
	_0("not") _0("&") _0("|") _0("^")  _0("&&") _0("||") _0("^^") _0("<<") _0(">>")

// I/O
	_0(".") _0("emit") _0("key") _0(".s")

	_0("") // double \0 ends the list
;

void dup() {
	data_t d  = peek();
	push(d);
}

void drop() {
	pop();
}

void swap() {
	data_t a  = pop();
	data_t b  = pop();
	push(a);
	push(b);
}

void rot() {
	data_t a  = pop();
	data_t b  = pop();
	data_t c  = pop();
    push(a);
	push(b);
	push(c);
}

char next_token[16];
void fetch_next_token() {
	next_token[sizeof(next_token)-1] = '\0';
	for (int i = 0; i < sizeof(next_token-1); i ++) {
		char next_char = getchar();
		next_token[i] = next_char;

		if (isspace(next_char)) {
			next_token[i] = '\0';
			break;
		}
	}
}

const char *find_next_opcode(const char *current_opcode) {
	while(*current_opcode) {
		current_opcode ++;
	}
	current_opcode ++;

	if (*current_opcode) return NULL; // test if double NUL
	return current_opcode;
}

opcode_t fetch_opcode() {
    opcode_t opcode = 0;
	const char *current_opcode = opcode_names;
    while(opcode < OPCODE_COUNT) {
		if (!strcmp(next_token, current_opcode)) {
			return opcode;
		}

		current_opcode = find_next_opcode(current_opcode);
		opcode ++;
    }
}

void unknown(opcode_t opcode) {
	printf("unknown opcode %d: %s\n", opcode, next_token);
}

void execute(opcode_t opcode)
{
	switch (opcode)
	{
	case OPCODE_DUP:
		dup();
		break;
	case OPCODE_DROP:
		drop();
		break;
	case OPCODE_SWAP:
		swap();
		break;
	case OPCODE_ROT:
		rot();
		break;

	default:
		unknown(opcode);
		break;
	}
}

void main() {
	opcode_t opcode = fetch_opcode();
	execute(opcode);
}
