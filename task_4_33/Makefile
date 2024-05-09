PROG = task_4_33 
CFLAGS = -Wall -g -ansi -pedantic
OBJMODULES = task_4_33.o operator.o op_stack.o num_stack.o

.PHONY: clean

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

$(PROG): $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm $(PROG) *.o
