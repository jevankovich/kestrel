#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>
#include <stddef.h>

#include "buffer.h"

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

const char *tok_asstr(token tok);
int isreserved(char *buf, size_t len);

typedef struct lex_state {
	FILE *in; // input stream
	int line, col; // position in stream
	buffer text; // buffer used to build token string
	int curr; // current character from input
	token tok; // the complete last token from the input string

	const char *errmsg; // Used to report errors during parsing
	token errtok;
} lex_state;

lex_state lex_init(FILE *in);

int next_tok(lex_state *l);

#endif
