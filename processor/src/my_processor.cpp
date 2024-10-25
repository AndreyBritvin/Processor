#include "my_processor.h"
// #include "my_settings.h"
#include "my_stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include <math.h>

static FILE *LOG_FILE = fopen("my_log.log", "w");

bool check_signature(char *signature, uint64_t version)
{
    assert(signature);

    return (strcmp(signature, SIGNATURE) == 0) && (version == CODE_VER);
}

err_code_t read_code(const char *input_filename, proc_code_t *code)
{
    assert(input_filename);
    assert(code);
    // FILE *input_file = fopen(input_filename, "r");
    SAFE_OPEN_FILE(input_file, input_filename, "r");

    fscanf(input_file, "%lu", &code->size);

    char signature[sizeof(SIGNATURE)] = {};
    uint64_t version = 0;

    fscanf(input_file, "%s",  signature);
    fscanf(input_file, "%lu", &version );

    if (!check_signature(signature, version))
    {
        printf("Please recompile code\n");
        return ERROR_SIGNATURE;
    }

    code->arr = (proc_val_t *) calloc(code->size, sizeof(proc_val_t));

    if (code->arr == NULL)
    {
        return ERROR_ALLOC;
    }

    proc_val_t command = 0;
    size_t instr_ptr = 0;

    while (fscanf(input_file, "%lf", &command) != EOF)
    {
        code->arr[instr_ptr++] = command;
    }

    fclose(input_file);

    return OK;
}

err_code_t run_code(proc_code_t code)
{
    proc_t proc = {};
    proc_ctor(&proc, code);

    bool is_valid_code = true;

    while (is_valid_code)
    {
        switch ((uint64_t) proc.code.arr[proc.instr_ptr] & CMD_MASK)
        {
            #define CMD_DESCR_DEF(ENUM_NAME, STR_NAME, ARG_TYPE, ...) \
            case ENUM_NAME:                                           \
            {                                                         \
                __VA_ARGS__                                           \
                break;                                                \
            }
            #include "../command_descriptions.cpp"

            default:
                processor_dump(proc);
                printf("Invalid instruction #%lu\n", (uint64_t) code.arr[proc.instr_ptr]);
                is_valid_code = false;
                break;
            #undef CMD_DESCR_DEF
        }
    }

    proc_dtor(&proc);

    return OK;
}

err_code_t proc_ctor(proc_t *proc, proc_code_t code)
{
    assert(proc);

    proc->code = code;
    proc->instr_ptr = 0;

    proc->stack = {};
    INIT_STACK(proc->stack);
    proc_val_t stack_poison_value = (proc_val_t) STACK_POISON_VALUE;
    stack_ctor(&proc->stack, DEFAULT_STACK_SIZE, sizeof(proc_val_t), print_doubles, &stack_poison_value);

    proc->ret_val_stack = {};
    INIT_STACK(proc->ret_val_stack);
    size_t stack_poison_value_ip = (size_t) STACK_POISON_VALUE;
    stack_ctor(&proc->ret_val_stack, DEFAULT_RET_STACK_SIZE, sizeof(size_t), print_longs, &stack_poison_value_ip);

    return OK;
}

err_code_t proc_dtor(proc_t *proc)
{
    assert(proc);

    stack_dtor(&proc->stack);
    stack_dtor(&proc->ret_val_stack);
    free(proc->code.arr);

    return OK;
}

err_code_t processor_dump(proc_t proc)
{
    printf("Code index: ");
    for (size_t instr_index = 0; instr_index < proc.code.size; instr_index++)
    {
        printf("%02lX ", instr_index);
    }

    printf("\nCode cmmnd: ");
    for (size_t instr_index = 0; instr_index < proc.code.size; instr_index++)
    {
        printf("%02llX ", (unsigned long long) proc.code.arr[instr_index]);
    }

    printf("\nWe`re here:");
    for (size_t instr_index = 0; instr_index < proc.instr_ptr; instr_index++)
    {
        printf("   ");
    }
    printf("  ^ ip = %lu\n", proc.instr_ptr);

    printf("\nStack dump: ");
    for (size_t stack_index = 0;  stack_index < proc.stack.size; stack_index++)
    {
        printf("%lg ", ((proc_val_t *)proc.stack.data)[stack_index]);
    }

    printf("\nRegisters dump: ");
    for (size_t reg_index = 0; reg_index < REGISTERS_COUNT; reg_index++)
    {
        printf("%cx ", (char) reg_index + 'a');
    }

    printf("\n                ");
    for (size_t reg_index = 0; reg_index < REGISTERS_COUNT; reg_index++)
    {
        printf("%02lX ", (uint64_t) proc.registers_arr[reg_index]);
    }

    printf("\n");
    print_ret_val_stack(proc.ret_val_stack);

    printf("\nRAM dump: ");
    for (size_t ram_index = 0;  ram_index < MAX_RAM_SIZE; ram_index++)
    {
        printf("%lg ", proc.ram[ram_index]);
    }
    printf("\n");

    return OK;
}

err_code_t print_RAM(proc_t proc)
{
    size_t length = sqrt(MAX_RAM_SIZE);
    size_t height = sqrt(MAX_RAM_SIZE);

    for (size_t line = 0; line < height; line++)
    {
        for (size_t row = 0; row < length; row++)
        {
            if ((uint64_t) proc.ram[line * length + row] == 0)
            {
                printf("-");
            }
            else
            {
                printf("*");
            }
        }

        printf("\n");
    }

    return OK;
}

proc_val_t* get_arg(proc_t *proc)
{
    assert(proc);

    proc_val_t  buffer   = 0;
    proc_val_t* ret_val  = 0;
    uint64_t    arg_type = (uint64_t) proc->code.arr[proc->instr_ptr++] & (~CMD_MASK);

    if (arg_type & REGISTER_VALUE ) {ret_val = &(proc->registers_arr[(uint64_t) proc->code.arr[proc->instr_ptr++]]);
                                     buffer  = *ret_val; }
    if (arg_type & IMMEDIATE_VALUE) {buffer +=  proc->code.arr[proc->instr_ptr++] ;
                                     ret_val = &buffer;}

    if (arg_type & RAM_VALUE)
    {
        ret_val = &(proc->ram[(uint64_t) buffer]);
    }
    // printf("Argtype = %d, ret_val = %p, ram_begin = %p\n", arg_type, ret_val, &proc->ram);

    return ret_val;
}

err_code_t print_ret_val_stack(my_stack_t ret_val_stack)
{
    printf("Printing ret val stack: ");
    for (size_t stack_index = 0; stack_index < ret_val_stack.size; stack_index++)
    {
        printf("%lg ", ((proc_val_t *)ret_val_stack.data)[stack_index]);
    }
    printf("\n");

    return OK;
}
