CFLAGS := -std=c99 -pedantic -Wall -O2

OBJDIR := obj
OBJS := kestrel.o buffer.o token.o intern.o
OBJS := $(addprefix $(OBJDIR)/, $(OBJS))

TESTDIR := test
TESTS := intern_test
TESTS := $(addprefix $(TESTDIR)/, $(TESTS))

kcc: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

obj/%.o: src/%.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<


obj/kestrel.o: src/buffer.h src/token.h
obj/buffer.o: src/buffer.h
obj/token.o: src/token.h

.PHONY: debug
debug: CFLAGS += -g -Og
debug: kcc

.PHONY: clean
clean:
	rm -f $(OBJS) $(TESTS) kcc

test/intern_test: src/intern.c
test/%: src/test/%.c src/test/test.h
	$(CC) $(CFLAGS) -Wno-unused-value $(CPPFLAGS) -o $@ $<
	$(TOOL) ./$@

.PHONY: test
test: $(TESTS)

.PHONY: bench
bench: CPPFLAGS += -DBENCH
bench: $(TESTS)

.PHONY: debugbench
debugbench: CFLAGS += -g
debugbench: bench
