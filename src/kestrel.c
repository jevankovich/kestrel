#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "buffer.h"
#include "token.h"

typedef struct lex_state {
	FILE *in; // input stream
	int line, col; // position in stream
	buffer text; // buffer used to build token string
	int curr; // current character from input
	token tok;
} lex_state;

lex_state lex_init(FILE *in) {
	buffer b = buf_init();

	return (lex_state){
		.in = in,
		.line = 1,
		.col = 1,
		.text = b,
		.curr = fgetc(in),
	};
}

void next(lex_state *l) {
	l->curr = fgetc(l->in);
	l->col++;
}

void keep(lex_state *l) {
	buf_append(&(l->text), l->curr);
	next(l);
}

void newline(lex_state *l) {
	assert(l->curr == '\n' || l->curr == '\r');
	char first = l->curr;
	next(l);
	if ((l->curr == '\n' || l->curr == '\r') && l->curr != first) {
		next(l);
	}

	l->line++;
	l->col = 1;
}

void number(lex_state *l) {
	while (isdigit(l->curr)) {
		keep(l);
	}
}

#define emit(l, t) (l)->tok.type = (t); return (t)
#define emitlong(l, t) (l)->tok.type = (t); (l)->tok.str = buf_asstr((l)->text); return (t)

int next_tok(lex_state *l) {
	int ret;
	buf_reset(l->text);

	for (;;) {
		l->tok.line = l->line;
		l->tok.col = l->col;

		switch(l->curr) {
			case '\n': case '\r':
				newline(l);
				break;
			case ' ': case '\t': case '\v': case '\f':
				next(l);
				break;
			case '0': case '1': case '2': case '3':
			case '4': case '5': case '6': case '7':
			case '8': case '9':
				number(l);
				emitlong(l, TK_NUMERAL);
			case EOF:
				return TK_EOS;
			default: // identifiers, keywords, operators, etc.
				if (isalpha(l->curr) || l->curr == '_') {
					keep(l);
					while(isalnum(l->curr) || l->curr == '_') {
						keep(l);
					}
					if ((ret = isreserved(buf_asbuflen(l->text))) >= 0) {
						emit(l, ret);
					} else {
						emitlong(l, TK_IDENT);
					}
				} else {
					ret = l->curr;
					next(l);
					emit(l, ret);
				}
		}
	}
}

void decl(lex_state *l) {
}

void parse(lex_state *l) {
	switch (next_tok(l)) {
		case TK_INT:
			decl(l);
			break;
	}
}

int main(int argc, char **argv) {
	FILE *in;
	if (argc > 1) {
		in = fopen(argv[1], "r");
		if (!in) {
			return 1;
		}
	} else {
		in = stdin;
	}

	lex_state l = lex_init(in);
	for (;;) {
		if (next_tok(&l) == TK_EOS) {
			break;
		}

		if (l.tok.type < FIRST_RESERVED) { // one character
			printf("%d,%d %c\n", l.tok.line, l.tok.col, l.tok.type);
		} else if (l.tok.type < TK_IDENT) { // reserved multi-character
			printf("%d,%d %s\n", l.tok.line, l.tok.col, tokstr(l.tok.type));
		} else { // user multi-character
			printf("%d,%d %s %s\n", l.tok.line, l.tok.col, tokstr(l.tok.type), l.tok.str);
		}
	}
}
