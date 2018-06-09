CFLAGS := -std=c99 -pedantic -Wall -O2

OBJDIR := obj
OBJS := kestrel.o buffer.o token.o
OBJS := $(addprefix $(OBJDIR)/, $(OBJS))

kcc: $(OBJS)
	$(CC) -o $@ obj/*.o $(LDFLAGS)

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
	rm -f $(OBJS) kcc
