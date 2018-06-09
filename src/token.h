#ifndef TOKEN_H
#define TOKEN_H

#include <stddef.h>

#define FIRST_RESERVED 256
#define NUM_RESERVED (TK_IDENT - FIRST_RESERVED)

enum token_type {
	TK_INT = FIRST_RESERVED, // int type
	TK_RET, // return
	TK_IDENT, TK_NUMERAL, // e.g. main, 0
	TK_EOS // End of token stream
};

#define tokstr(t) (TOKEN_STRINGS[(t) - FIRST_RESERVED])
extern const char * const TOKEN_STRINGS[];

typedef struct token {
	enum token_type type;
	int line, col; // The position in the input string
	char *str;
} token;


int isreserved(char *buf, size_t len);

#endif
