#ifndef MY_PROCESSOR_H_
#define MY_PROCESSOR_H_

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
};

int run_code(proc_code_t *code);
int read_code(char *input_filename, proc_code_t *code);

#endif // MY_PROCESSOR_H_
