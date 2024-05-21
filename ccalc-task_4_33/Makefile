PROG = ccalc 
ASM = nasm
LD = ld
CFLAGS = -Wall -g -m32
ASMFLAGS = -g
OBJMODULES = ccalc.o operator.o stack.o history_buffer.o #syscall.o

UNAME_S = $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	ASMFLAGS += -f elf32 -dOS_LINUX
	LDFLAGS = -m elf_i386
endif
ifeq ($(UNAME_S),FreeBSD)
	ASMFLAGS += -f elf32 -dOS_LINUX
	LDFLAGS = -m elf_i386
endif
ifeq ($(UNAME_S),Darwin)
	ASMFLAGS += -f macho -dOS_FREEBSD -g
	LDFLAGS = -lSystem -macosx_version_min 10.13
endif

.PHONY: clean

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(PROG): $(OBJMODULES)
	$(LD) $(LDFLAGS) $^ -o $@

clean:
	rm $(PROG) *.o
