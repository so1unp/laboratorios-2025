CC=gcc
BIN=./bin
CFLAGS=-Wall -g -pthread -Werror

PROG=say threads benchmark glob sched-threads

LIST=$(addprefix $(BIN)/, $(PROG))

.PHONY: all
all: $(LIST)

$(BIN)/%: %.c
	$(CC) -o $@ $< $(CFLAGS)

%: %.c
	$(CC) -o $(BIN)/$@ $< $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(LIST)
