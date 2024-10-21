NAME = circle

all:
	make -C assembler
	assembler/assembler.out program_codes/$(NAME).txt compiled/$(NAME)_asm.txt

	make -C processor
	processor/proc.out compiled/$(NAME)_asm.txt
