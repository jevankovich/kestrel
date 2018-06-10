#ifndef INTERN_H
#define INTERN_H

#include <stddef.h>

typedef struct intern *intern;

void intern_init(intern, size_t);
void intern_free(intern);

const char *intern_add(intern, const char *);
const char *intern_addn(intern , const char *, size_t);

#endif
