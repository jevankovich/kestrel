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
	token tok; // the complete last token from the input string
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

void expr(lex_state *l) {
	if (next_tok(l) != TK_NUMERAL) {
		fprintf(stderr, "Expected numeric literal");
		exit(1);
	}
	printf("%s", l->tok.str);

	if (next_tok(l) != ';') {
		fprintf(stderr, "Expected ;");
		exit(1);
	}
	printf(";\n");
}

void statement(lex_state *l) {
	if (next_tok(l) != TK_RET) {
		fprintf(stderr, "Expected return\n");
		exit(1);
	}
	printf("    return ");

	expr(l);
}

// {...}
void block(lex_state *l) {
	if (next_tok(l) != '{') {
		fprintf(stderr, "Expected right bracket\n");
		exit(1);
	}
	printf("{\n");

	statement(l);

	if (next_tok(l) != '}') {
		fprintf(stderr, "Expected right bracket\n");
		exit(1);
	}
	printf("}\n");
}

// int ident() {...}
void decl(lex_state *l) {
	assert(l->tok.type == TK_INT);
	printf("int ");
	if (next_tok(l) != TK_IDENT) {
		fprintf(stderr, "Type specification must be followed by an identifier\n");
		exit(1);
	}

	char *id = l->tok.str;
	printf("%s", id);

	if (!(next_tok(l) == '(' && next_tok(l) == ')')) {
		fprintf(stderr, "Malformed declaration\n");
		exit(1);
	}
	printf("()");

	block(l);
}

void parse(lex_state *l) {
	switch (next_tok(l)) {
		case TK_INT:
			decl(l);
			break;
		default:
			fprintf(stderr, "Unexpected token in source");
			exit(1);
	}
}

int main(int argc, char **argv) {
	FILE *in;
	if (argc > 1) {
		in = fopen(argv[1], "r");
		if (!in) {
			perror(NULL);
			return 1;
		}
	} else {
		in = stdin;
	}

	lex_state l = lex_init(in);
	parse(&l);
}
