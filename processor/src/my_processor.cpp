#include "my_processor.h"
// #include "my_settings.h"
#include "my_stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include <math.h>

static FILE *LOG_FILE = fopen("my_log.log", "w");

bool check_signature(char *signature, int version)
{
    return (strcmp(signature, SIGNATURE) == 0) && (version == CODE_VER);
}

err_code_t read_code(const char *input_filename, proc_code_t *code)
{
    // FILE *input_file = fopen(input_filename, "r");
    SAFE_OPEN_FILE(input_file, input_filename, "r");

    fscanf(input_file, "%llu", &code->size);

    char signature[sizeof(SIGNATURE)] = {};
    uint64_t version = 0;

    fscanf(input_file, "%s",  signature);
    fscanf(input_file, "%llu", &version);

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

    while (fscanf(input_file, "%lld", &command) != EOF)
    {
        code->arr[instr_ptr++] = command;
    }

    fclose(input_file);

    return OK;
}

err_code_t run_code(proc_code_t code)
{
    proc_t proc = {};
    proc.code = code;
    proc.instr_ptr = 0;

    proc.stack = {};
    INIT_STACK(proc.stack);
    proc_val_t stack_poison_value = (proc_val_t) STACK_POISON_VALUE;
    stack_ctor(&proc.stack, DEFAULT_STACK_SIZE, sizeof(proc_val_t), print_longs, &stack_poison_value);

    proc.ret_val_stack = {};
    INIT_STACK(proc.ret_val_stack);
    size_t stack_poison_value_ip = (size_t) STACK_POISON_VALUE;
    stack_ctor(&proc.ret_val_stack, DEFAULT_RET_STACK_SIZE, sizeof(size_t), print_longs, &stack_poison_value_ip);

    bool is_valid_code = true;

    while (is_valid_code)
    {
        switch (proc.code.arr[proc.instr_ptr] & CMD_MASK)
        {
            #define COMMAND_DESCR(ENUM_NAME, STR_NAME, ASS_CODE, ...) \
            case ENUM_NAME:                                           \
            {                                                         \
                __VA_ARGS__                                           \
                break;                                                \
            }
            #include "../command_descriptions.cpp"
        /*
            case HLT:
            {
                is_valid_code = false;
                break;
            }
            case PUSH:
            {
                proc_val_t to_push = code[proc.instr_ptr + 1];
                stack_push(&progr_stack, &to_push);
                proc.instr_ptr += 2;
                break;
            }
            case ADD:
            {
                proc_val_t  first_add = 0;
                proc_val_t second_add = 0;
                stack_pop(&progr_stack, &second_add);
                stack_pop(&progr_stack,  &first_add);
                proc_val_t to_add = first_add + second_add;
                stack_push(&progr_stack, &to_add);
                proc.instr_ptr += 1;
                break;
            }
            case OUT:
            {
                proc_val_t to_out = 0;
                stack_pop(&progr_stack, &to_out);
                printf("Processor out: %lld\n", to_out);
                proc.instr_ptr += 1;
                break;
            }
            case DUMP:
            {
                // STACK_DUMP(&progr_stack);
                processor_dump(proc);
                proc.instr_ptr += 1;
                break;
            }
            case JUMP:
            {
                proc.instr_ptr = (size_t) proc.code[proc.instr_ptr + 1];
                break;
            }
*/
            default:
                printf("Invalid instruction #%lld\n", code.arr[proc.instr_ptr]);
                is_valid_code = false;
                break;
            #undef COMMAND_DESCR
        }
    }

    stack_dtor(&proc.stack);
    stack_dtor(&proc.ret_val_stack);
    free(code.arr);

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
        printf("%lld ", ((proc_val_t *)proc.stack.data)[stack_index]);
    }

    printf("\nRegisters dump: ");
    for (size_t reg_index = 0; reg_index < REGISTERS_COUNT; reg_index++)
    {
        printf("%cx ", reg_index + 'a');
    }

    printf("\n                ");
    for (size_t reg_index = 0; reg_index < REGISTERS_COUNT; reg_index++)
    {
        printf("%02llX ", (unsigned long long) proc.registers_arr[reg_index]);
    }

    printf("\n");
    print_ret_val_stack(proc.ret_val_stack);

    printf("\nRAM dump: ");
    for (size_t ram_index = 0;  ram_index < MAX_RAM_SIZE; ram_index++)
    {
        printf("%lld ", proc.ram[ram_index]);
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
            if (proc.ram[line * length + row] == 0)
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

proc_val_t* get_arg(proc_t *proc) // TODO redo algorithm
{
    proc_val_t  buffer   = 0;
    proc_val_t* ret_val  = 0;
    proc_val_t  arg_type = proc->code.arr[proc->instr_ptr++] & (~CMD_MASK);

    if (arg_type & REGISTER_VALUE ) {ret_val = &(proc->registers_arr[proc->code.arr[proc->instr_ptr++]]);
                                     buffer  = *ret_val; }
    if (arg_type & IMMEDIATE_VALUE) {buffer +=  proc->code.arr[proc->instr_ptr++] ;
                                     ret_val = & buffer;}

    if (arg_type & RAM_VALUE)
    {
        ret_val = &(proc->ram[buffer]);
    }

    return ret_val;
}

err_code_t print_ret_val_stack(my_stack_t ret_val_stack)
{
    printf("Printing ret val stack: ");
    for (size_t stack_index = 0; stack_index < ret_val_stack.size; stack_index++)
    {
        printf("%lld ", ((proc_val_t *)ret_val_stack.data)[stack_index]);
    }
    printf("\n");

    return OK;
}
