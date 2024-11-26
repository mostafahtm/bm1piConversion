# Add additional compiler flags here
CFLAGS=-std=c17 -O3 -msse4.1

.PHONY: all
all: main
main: main.c conversion.c
	$(CC) $(CFLAGS) -o $@ $^
# "$@" copy pastes target name "main" in the command below
# "$^"  copy pastes dependency names "main.c", "conversion.c" in the command below

clean:
	rm -f main
