#include "token.h"

#include <string.h>

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
