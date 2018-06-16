#include "intern.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define DENOM 4
#define NUMER 3
#define FILL_RATIO NUMER/DENOM


int intern_init(intern *i, size_t cap) {
	cap = cap * DENOM / NUMER; // i.e. to store 3, we need 4
	if (cap < DENOM) { cap = DENOM; }
	i->data = calloc(sizeof(i->data[0]), cap);
	if (!(i->data)) {
		return -1;
	}

	i->filled = 0;
	i->cap = cap;
	return 0;
}

void intern_free(intern i) {
	for (size_t idx = 0; idx < i.cap; idx++) {
		free(i.data[idx]);
	}

	free(i.data);
}

static size_t find(intern i, uint64_t h, const char *str) {
	h %= i.cap;
	uint64_t idx = h;
	for (;;) {
		if (i.data[idx] == NULL) {
			return idx;
		}

		if (strcmp(i.data[idx], str) == 0) {
			return idx;
		}

		idx++;
		idx %= i.cap;
	}
}

static uint64_t hash(const char *str) {
	// FNV-1a hash for now
	static const uint64_t prime = ((uint64_t)1<<40) + ((uint64_t)1<<8) + (uint64_t)0xb3;
	uint64_t h = 0xcbf29ce484222325;
	while (*str != 0) {
		h ^= *str++;
		h *= prime;
	}
	return h;
}

static void add_unchecked(intern *i, char *str) {
	i->data[find(*i, hash(str), str)] = str;
}

static void grow(intern *i) {
	char **old = i->data;
	i->data = calloc(sizeof(i->data[0]), i->cap * 2);
	if (!(i->data)) {
		fprintf(stderr, "Out of memory\n");
		exit(1);
	}

	for (size_t idx = 0; idx < i->cap; idx++) {
		if (old[idx]) {
			add_unchecked(i, old[idx]);
		}
	}

	free(old);
	i->cap *= 2;
}

const char *intern_add(intern *i, const char *str) {
	size_t len = strlen(str);

	return intern_addn(i, str, len);
}

const char *intern_addn(intern *i, const char *buf, size_t len) {
	uint64_t h = hash(buf);
	size_t ret = find(*i, h, buf);
	if (i->data[ret]) {
		return i->data[ret];
	} else if (i->filled >= i->cap * FILL_RATIO) {
		grow(i);
	}

	char *newbuf = malloc(len + 1);
	memcpy(newbuf, buf, len);
	newbuf[len] = 0;

	i->data[find(*i, h, buf)] = newbuf;

	i->filled++;
	return newbuf;
}

const char *intern_find(intern i, const char *str) {
	size_t len = strlen(str);

	return intern_findn(i, str, len);
}

const char *intern_findn(intern i, const char *buf, size_t len) {
	uint64_t h = hash(buf);
	return i.data[find(i, h, buf)];
}
