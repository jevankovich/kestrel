#include "token.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "buffer.h"

// Make sure this matches with enum token_type
const char * const TOKEN_STRINGS[] = {
	"int", "return", 
	"<ident>", "<numeral>", "<eos>"};

int isreserved(char *buf, size_t len) {
	for (int i = 0; i < NUM_RESERVED; i++) {
		if (strncmp(buf, TOKEN_STRINGS[i], len) == 0) {
			return i + FIRST_RESERVED;
		}
	}
	return -1;
}

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

static void next(lex_state *l) {
	l->curr = fgetc(l->in);
	l->col++;
}

static void keep(lex_state *l) {
	buf_append(&(l->text), l->curr);
	next(l);
}

static void newline(lex_state *l) {
	assert(l->curr == '\n' || l->curr == '\r');
	char first = l->curr;
	next(l);
	if ((l->curr == '\n' || l->curr == '\r') && l->curr != first) {
		next(l);
	}

	l->line++;
	l->col = 1;
}

static void number(lex_state *l) {
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
