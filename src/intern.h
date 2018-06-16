#ifndef INTERN_H
#define INTERN_H

#include <stddef.h>

typedef struct intern {
	size_t filled;
	size_t cap;
	char **data;
} intern;

int intern_init(intern *, size_t);
void intern_free(intern);

const char *intern_add(intern *, const char *);
const char *intern_addn(intern *, const char *, size_t);

const char *intern_find(intern, const char *);
const char *intern_findn(intern, const char *, size_t);

#endif
