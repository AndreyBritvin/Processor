all:
	make assembler/Makefile
	assembler/assembler.out program_codes/ram_exp.txt compiled/ram_exp_asm.txt

	make processor/Makefile
	processor/proc.out compiled/ram_exp_asm.txt
