all:
	nasm -f elf -F dwarf helloworld.asm -g
	ld -m elf_i386 -s -g -o helloworld helloworld.o
