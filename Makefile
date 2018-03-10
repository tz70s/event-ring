CC=gcc
CFLAGS=-O3 -Wall -Werror

all: pre_build out/events out/server out/client 

pre_build:
	@mkdir -p out

out/events: src/event.c
	$(CC) $(CFLAGS) -o $@ $<

out/server: src/server.c
	$(CC) $(CFLAGS) -o $@ $<

out/client: src/client.c
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -rf out