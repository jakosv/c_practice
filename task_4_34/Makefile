PROG = task_4_34 
ASM = nasm
LD = ld
CFLAGS = -Wall -ansi -pedantic -g -nostdlib
ASMFLAGS = -g
OBJMODULES = task_4_34.o syscall.o util.o

UNAME_S = $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	ASMFLAGS += -f elf32 -dOS_LINUX
	LDFLAGS = -m elf_i386
endif
ifeq ($(UNAME_S),FreeBSD)
	ASMFLAGS += -f elf32 -dOS_FREEBSD
	LDFLAGS = -m elf_i386
endif
ifeq ($(UNAME_S),Darwin)
	ASMFLAGS += -f macho -dOS_DARWIN -g
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
