#include "../intern.c"

#include "test.h"

char *str1 = "a";
char *str2 = "a\0";

char *strs[] = {
	"a",
	"b",
	"c",
	"d",
	"f"};

void test_init(intern *i) {
	test(!intern_init(i, 0), "Initialize intern set");
	test(i->cap, "Capacity isn't zero");
}

void test_add(intern *in) {
	for (int i = 0; i < 10; i++) {
		intern_add(in, str1);
	}

	test(in->filled == 1, "Only one is added");

	test(intern_add(in, str1) == intern_add(in, str1), "Consistent access");

	int count = 0;
	for (int i = 0; i < in->cap; i++) {
		if (in->data[i] != NULL) {
			count++;
		}
	}

	test(count == 1, "Only one is actually added");
}

void test_grow(intern *in) {
	for (int i = 0; i < (sizeof(strs) / sizeof(strs[0])); i++) {
		intern_add(in, strs[i]);
	}

	test(in->filled == 5, "All were added");

	for (int i = 0; i < (sizeof(strs) / sizeof(strs[0])); i++) {
		const char *interned = intern_add(in, strs[i]);
		test(strcmp(strs[i], interned) == 0, "Actually in there");
	}
}

void bench_lookup_setup(int n, void *user) {
	static char buf[11];
	intern *in = (intern *)user;
	intern_init(in, n);

	for (int i = 0; i < n; i++) {
		snprintf(buf, sizeof(buf), "%d", i);
		intern_add(in, buf);
	}
}

void bench_lookup_run(int n, void *user) {
	static char buf[11];
	intern *in = (intern *)user;

	for (int i = 0; i < n; i++) {
		snprintf(buf, sizeof(buf), "%d", i);
		intern_find(*in, buf);
	}
}

void bench_add_setup(int n, void *user) {
	intern_init((intern *)user, 0);
}

void bench_add_setup_prealloc(int n, void *user) {
	intern_init((intern *)user, n);
}

void bench_add_run(int n, void *user) {
	static char buf[11];
	intern *in = (intern *)user;

	for (int i = 0; i < n; i++) {
		snprintf(buf, sizeof(buf), "%d", i);
		intern_add(in, buf);
	}
}

void bench_cleanup(void *user) {
	intern_free(*(intern *)user);
}

int main() {
	intern i;

	test_init(&i);
	test(str1 != str2, "Distinct strings");
	test(hash(str1) == hash(str2), "Same hash");
	test_add(&i);

	intern_free(i);
	bench("intern find", bench_lookup_setup, bench_lookup_run, bench_cleanup, &i);

	bench("intern add (no preallocate)", bench_add_setup, bench_add_run, bench_cleanup, &i);
	bench("intern add (preallocate)", bench_add_setup_prealloc, bench_add_run, bench_cleanup, &i);
}
