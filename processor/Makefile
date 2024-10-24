CC = g++

HEADERS_INCLUDE = -Iinclude -I../../Stack/include -I../include

#CFLAGS = $(HEADERS_INCLUDE) -lm -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ 						\
							-Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations 			\
							-Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported 			\
							-Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral \
							-Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op 			\
							-Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith  \
							-Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wswitch-default 		\
							-Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn 		\
							-Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wsign-promo   \
							-Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast	\
							-Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers 			\
							-Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new 	\
							-fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging	\
							-fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 					\
							-pie -fPIE -Werror=vla -fsanitize=address,alignment,bool,bounds,enum,			\
							float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,			\
							nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,		\
							signed-integer-overflow,undefined,unreachable,vla-bound,vptr

CFLAGS = $(HEADERS_INCLUDE) -lm -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE -Werror=vla -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr


all: proc.out

proc.out: build/main.o build/my_processor.o ../build/my_stack.o ../build/my_stack_protection.o ../build/my_stack_print.o #build/my_log.o
	@$(CC) $(CFLAGS) -o proc.out build/main.o build/my_processor.o ../build/my_stack.o ../build/my_stack_protection.o ../build/my_stack_print.o #build/my_log.o

build/main.o: src/main.cpp
	@$(CC) $(CFLAGS) -c src/main.cpp -o build/main.o

../build/my_stack.o: ../../Stack/src/my_stack.cpp ../../Stack/include/my_stack.h
	@$(CC) $(CFLAGS) -c ../../Stack/src/my_stack.cpp -o ../build/my_stack.o

../build/my_stack_protection.o: ../../Stack/src/my_stack_protection.cpp ../../Stack/include/my_stack_protection.h
	@$(CC) $(CFLAGS) -c ../../Stack/src/my_stack_protection.cpp -o ../build/my_stack_protection.o

../build/my_stack_print.o: ../../Stack/src/my_stack_print.cpp ../../Stack/include/my_stack.h
	@$(CC) $(CFLAGS) -c ../../Stack/src/my_stack_print.cpp -o ../build/my_stack_print.o

build/my_processor.o: src/my_processor.cpp include/my_processor.h ../command_descriptions.cpp ../include/commands_enum.h
	@$(CC) $(CFLAGS) -c src/my_processor.cpp -o build/my_processor.o

build/my_log.o: ../src/my_log.cpp
	@$(CC) $(CFLAGS) -c ../src/my_log.cpp -o build/my_log.o

run:
	./proc.out ../compiled/comp_test.txt

run_reg:
	./proc.out ../compiled/registers_code_asm.txt

run_jump:
	./proc.out ../compiled/jump_code_asm.txt

run_ram:
	./proc.out ../compiled/ram_exp_asm.txt

clean:
	rm proc.out

