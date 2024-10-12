#include "my_processor.h"
// #include "my_settings.h"
#include "my_log.h"
#include "my_stack.h"
#include <stdio.h>
#include <stdlib.h>

static FILE *LOG_FILE = fopen("my_log.log", "w");

int read_code(char *input_filename, proc_code_t *code)
{
    FILE *input_file = fopen(input_filename, "r");

    proc_val_t command = 0;
    size_t instr_ptr = 0;

    while (fscanf(input_file, "%lld", &command) != EOF)
    {
        code[instr_ptr++] = command;
    }

    return instr_ptr;
}

int run_code(proc_code_t *code)
{
    proc_t proc = {};
    proc.code = code;
    proc.code_size = 20; // TODO: make auto text file
    proc.instr_ptr = 0;

    my_stack_t progr_stack = {};
    INIT_STACK(progr_stack);
    proc_val_t stack_poison_value = (proc_val_t) STACK_POISON_VALUE;
    stack_ctor(&progr_stack, 32, sizeof(proc_val_t), print_longs, &stack_poison_value);

    bool is_valid_code = true;

    while (is_valid_code)
    {
        switch (code[proc.instr_ptr])
        {
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

            default:
                printf("Invalid instruction #%lld\n", code[proc.instr_ptr]);
                is_valid_code = false;
                break;
        }
    }

    stack_dtor(&progr_stack);

    return 0;
}

int processor_dump(proc_t proc)
{
    printf("Code index: ");
    for (size_t instr_index = 0; instr_index < proc.code_size; instr_index++)
    {
        printf("%02X ", instr_index);
    }

    printf("\nCode cmmnd: ");
    for (size_t instr_index = 0; instr_index < proc.code_size; instr_index++)
    {
        printf("%02X ", proc.code[instr_index]);
    }

    printf("\nWe`re here:");
    for (size_t instr_index = 0; instr_index < proc.instr_ptr; instr_index++)
    {
        printf("   ");
    }
    printf("  ^ ip = %d\n\n", proc.instr_ptr);

    printf("Stack dump: ");
    for (size_t stack_index = 0;  stack_index < proc.stack.size; stack_index++)
    {
        printf("%d ", proc.stack.data + stack_index * proc.stack.elem_size);
    }

    return 0;
}
