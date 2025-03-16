all: lc4

lc4_disassembler.o: lc4_disassembler.c
	clang -c -g lc4_disassembler.c
	
lc4_loader.o: lc4_loader.c
	clang -c -g lc4_loader.c
	
lc4_memory.o: lc4_memory.c
	clang -c -g lc4_memory.c

lc4: lc4.c lc4_memory.o lc4_loader.o lc4_disassembler.o
	clang lc4.c lc4_memory.o lc4_loader.o lc4_disassembler.o -g -o lc4

clean:
	rm -f *.o

clobber: clean
	rm -f lc4