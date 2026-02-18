CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 -O2
SRCDIR = src
TESTDIR = tests
OUTDIR = output

# Source files
VEC3_OBJS = $(SRCDIR)/vec3.o
TEST_VEC3_SRC = $(TESTDIR)/test_vec3.c

.PHONY: all clean test

all: vibe_tracing

# Main program target (will be used in later steps)
vibe_tracing: $(VEC3_OBJS) $(SRCDIR)/main.o
	$(CC) $(CFLAGS) -o $@ $^ -lm

$(SRCDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Unit tests
test: test_vec3

test_vec3: $(VEC3_OBJS) $(TESTDIR)/test_vec3.o
	$(CC) $(CFLAGS) -o $@ $^ -lm
	./test_vec3

$(TESTDIR)/%.o: $(TESTDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(SRCDIR)/*.o $(TESTDIR)/*.o vibe_tracing test_vec3
	rm -f $(OUTDIR)/*.ppm
