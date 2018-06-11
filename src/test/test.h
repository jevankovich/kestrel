#ifndef TEST_H
#define TEST_H

#include <assert.h>

#ifdef BENCH
#include <time.h>
#include <stdio.h>

#define MIN_TIME (CLOCKS_PER_SEC / 10)
#endif

#define test(cond, msg) assert((msg, cond))

static void bench(const char *name, void (*setup)(int, void *), void (*run)(int, void *), void (*cleanup)(void *), void *user)
#ifdef BENCH
{
	int n = 64;
	clock_t start, finish;
	clock_t delta;

	do {
		n*=2;
		if (setup) {
			setup(n, user);
		}
		start = clock();
		run(n, user);
		finish = clock();
		if (cleanup) {
			cleanup(user);
		}

		delta = finish - start;
	} while (delta < MIN_TIME);
	
	fprintf(stderr, "\"%s\": %d runs in %f ms\n    %f us per run\n",
			name, n, (double)delta / CLOCKS_PER_SEC * 1000, 
			(double)delta / CLOCKS_PER_SEC / n * 1000000);
}
#else
{}
#endif

#endif
