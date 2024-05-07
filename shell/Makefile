PROG = myshell
CFLAGS = -Wall -g -ansi -pedantic
OBJMODULES = word_list.o word_parser.o

.PHONY: clean install

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

$(PROG): main.c $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@

install:
	install $(PROG) /usr/local/bin

clean:
	rm $(PROG) *.o
