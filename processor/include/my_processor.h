#ifndef MY_PROCESSOR_H_
#define MY_PROCESSOR_H_
#include <stdlib.h>
#include <my_stack.h>

static const char        *SIGNATURE = "Lychok";
static const int           CODE_VER = 8;
static const size_t REGISTERS_COUNT = 16;
static const int           CMD_MASK = 0b00011111;
static const size_t MAX_COMMAND_LEN = 20;
static const size_t    MAX_RAM_SIZE = 25;

static const size_t DEFAULT_RET_STACK_SIZE = 32;
static const size_t DEFAULT_STACK_SIZE     = 32;

typedef long long proc_val_t;

enum proc_errors
{
    // SUCCESS         = 0,
    ERROR_SIGNATURE = 1,
    ERROR_ALLOC = 2,
};

enum proc_coommands_argument
{
    IMMEDIATE_VALUE = 0b001 << 5,
    REGISTER_VALUE  = 0b010 << 5,
    RAM_VALUE       = 0b100 << 5,
};

enum processor_commands
{
/*
    #define COMMAND_DESCR(ENUM_NAME, STR_NAME, ASS_CODE, ...) ENUM_NAME,

    #include "../command_descriptions.cpp"
    #undef COMMAND_DESCR*/
    HLT      =  0,
    PUSH     =  1,
    ADD      =  2,
    SUB      =  3,
    MUL      =  4,
    DIV      =  5,
    OUT      =  6,
    IN       =  7,
    SQRT     =  8,
    SIN      =  9,
    COS      = 10,
    DUMP     = 11,
    POP      = 12,
    JMP      = 13,
    JA       = 14,
    GPU      = 15,
    CALL     = 16,
    RET      = 17,
};

struct proc_code_t
{
    size_t         size;
    proc_val_t     *arr;
};

struct proc_t
{
    size_t                            instr_ptr;
    proc_code_t                            code;
    proc_val_t   registers_arr[REGISTERS_COUNT];
    my_stack_t                            stack;
    my_stack_t                    ret_val_stack;
    proc_val_t                ram[MAX_RAM_SIZE];
};

int run_code(proc_code_t code);
bool check_signature(char *signature, int version);

int read_code(const char *input_filename, proc_code_t *code);
int processor_dump(proc_t proc);

int print_RAM(proc_t proc);
int print_ret_val_stack(my_stack_t ret_val_stack);

proc_val_t  get_arg_push(proc_t *proc);
proc_val_t* get_arg_pop (proc_t *proc);


#endif // MY_PROCESSOR_H_
