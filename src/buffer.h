#ifndef BUFFER_H
#define BUFFER_H

#include <stddef.h>
#include <string.h>

typedef struct buffer {
	char *buf;
	size_t len;
	size_t size;
} buffer;

#define buf_asbuflen(b) (b).buf, (b).len

buffer buf_init();

#define buf_reset(b) ((b).len = 0)
void buf_append(buffer *b, char c);

char *buf_asstr(buffer b);
#define buf_strcmp(b, str) (strncmp((b).buf, (str), (b).len))

#endif
