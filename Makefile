# PROG_NAME = factorial_v2

all:
	make -C assembler
	assembler/assembler.out program_codes/$(PROG_NAME).txt compiled/$(PROG_NAME)_asm.txt

	make -C processor
	processor/proc.out compiled/$(PROG_NAME)_asm.txt
