#include "buffer.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

buffer buf_init() {
	char *b = malloc(1);
	if (!b) {
		fprintf(stderr, "Out of memory\n");
		exit(1);
	}

	return (buffer){
		.buf = b,
		.len = 0,
		.size = 1,
	};
}

void buf_resize(buffer *b, size_t newsize) {
	char *newbuf = realloc(b->buf, newsize);
	if (!newbuf) {
		fprintf(stderr, "Out of memory\n");
		exit(1);
	}

	b->buf = newbuf;
	b->size = newsize;
}

void buf_append(buffer *b, char c) {
	if (b->len == b->size) {
		buf_resize(b, b->size * 2);
	}

	b->buf[b->len++] = c;
}

char *buf_asstr(buffer b) {
	char *str = malloc(b.len + 1);
	if (str == NULL) {
		fprintf(stderr, "Out of memory\n");
		exit(1);
	}

	memcpy(str, b.buf, b.len);
	str[b.len] = 0;

	return str;
}
