#ifndef MY_PROCESSOR_H_
#define MY_PROCESSOR_H_
#include <stdlib.h>
#include <my_stack.h>

const char *SIGNATURE = "Lychok";
const int   CODE_VER  = 1;

typedef long long proc_code_t;
typedef long long proc_val_t;

enum processor_commands
{
    HLT      =  0,
    PUSH     =  1,
    ADD      =  2,
    SUB      =  3,
    MULT     =  4,
    DIV      =  5,
    OUT      =  6,
    IN       =  7,
    SQRT     =  8,
    SIN      =  9,
    COS      = 10,
    DUMP     = 11,
    JUMP     = 12,
};

struct proc_t
{
    size_t               code_size;
    size_t               instr_ptr;
    proc_val_t               *code;
    my_stack_t               stack;
    proc_val_t      *registers_arr;
};

int run_code(proc_code_t *code);
size_t read_code(char *input_filename, proc_code_t *code);
int processor_dump(proc_t proc);

#endif // MY_PROCESSOR_H_
