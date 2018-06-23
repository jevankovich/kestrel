#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "token.h"

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
