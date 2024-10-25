#ifndef MY_PROCESSOR_H_
#define MY_PROCESSOR_H_
#include <stdlib.h>
#include <my_stack.h>
#include <stdint.h>
#include "commands_enum.h"
#include "utils.h"

static const char       SIGNATURE[] = "LychokBest";
static const uint64_t      CODE_VER = 12;

static const size_t REGISTERS_COUNT = 16;
static const int           CMD_MASK = 0b00011111;
static const size_t MAX_COMMAND_LEN = 128;
static const size_t    MAX_RAM_SIZE = 21 * 21;

static const size_t DEFAULT_RET_STACK_SIZE = 32;
static const size_t DEFAULT_STACK_SIZE     = 32;

typedef double proc_val_t;

enum proc_errors
{
    // SUCCESS         = 0,
    ERROR_SIGNATURE = 1,
};

enum proc_coommands_argument
{
    IMMEDIATE_VALUE = 0b001 << 5,
    REGISTER_VALUE  = 0b010 << 5,
    RAM_VALUE       = 0b100 << 5,
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

err_code_t run_code(proc_code_t code);
bool check_signature(char *signature, int version);

err_code_t read_code(const char *input_filename, proc_code_t *code);
err_code_t proc_ctor(proc_t *proc, proc_code_t code);
err_code_t proc_dtor(proc_t *proc);
err_code_t processor_dump(proc_t proc);

err_code_t print_RAM(proc_t proc);
err_code_t print_ret_val_stack(my_stack_t ret_val_stack);

proc_val_t* get_arg(proc_t *proc);

#endif // MY_PROCESSOR_H_
