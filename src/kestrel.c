#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "token.h"

#define error(l, str) do {\
	(l)->errmsg = (str);\
	(l)->errtok = (l)->tok;\
	return 1;\
} while (0)

static int expect(lex_state *l, int tok_type) {
	if (l->tok.type != tok_type) {
		return 1;
	}

	next_tok(l);
	return 0;
}

static int expr(lex_state *l) {
	if (expect(l, TK_NUMERAL)) {
		error(l, "Expected numeric literal");
	}
	printf("%s", l->tok.str);

	return 0;
}

static int statement(lex_state *l) {
	if (expect(l, TK_RET)) {
		error(l, "Expected \"return\"");
	}
	printf("    return ");

	if (expr(l)) {
		return 1;
	}

	if (expect(l, ';')) {
		error(l, "Expected ';'");
	}
	printf(";\n");

	return 0;
}

// {...}
static int block(lex_state *l) {
	if (expect(l, '{')) {
		error(l, "Expected '{'");
	}
	printf("{\n");

	if (statement(l)) {
		return 1;
	}

	if (expect(l, '}')) {
		error(l, "Expected '}'");
	}
	printf("}\n");

	return 0;
}

// int ident() {...}
static int decl(lex_state *l) {
	if (expect(l, TK_INT)) {
		error(l, "Expected a declaration");
	}
	printf("int ");

	if (expect(l, TK_IDENT)) {
		error(l, "Expected an identifier");
	}

	char *id = l->tok.str;
	printf("%s", id);

	if (expect(l, '(') || expect(l, ')')) {
		error(l, "Expected \"()\"");
	}
	printf("() ");

	if (block(l)) {
		return 1;
	}

	return 0;
}

int parse(lex_state *l) {
	next_tok(l);
	while (l->tok.type != TK_EOS) {
		if (decl(l)) {
			return 1;
		}
	}

	return 0;
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
	if (parse(&l)) {
		fprintf(stderr, "%d:%d: Found \"%s\"\n\t%s\n", 
				l.errtok.line, l.errtok.col, tok_asstr(l.errtok), l.errmsg);
		return 1;
	}
}
