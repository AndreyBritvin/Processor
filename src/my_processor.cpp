#include "my_processor.h"
#include "my_stack.h"
// #include "my_macros.h"
#include <stdio.h>
#include <stdlib.h>

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
    my_stack_t progr_stack = {};
    INIT_STACK(progr_stack);
    proc_val_t stack_poison_value = (proc_val_t) STACK_POISON_VALUE;
    stack_ctor(&progr_stack, 32, sizeof(proc_val_t), print_longs, &stack_poison_value);

    bool is_valid_code = true;
    int instr_ptr = 0;

    while (is_valid_code)
    {
        switch (code[instr_ptr])
        {
            case HLT:
            {
                is_valid_code = false;
                break;
            }
            case PUSH:
            {
                proc_val_t to_push = code[instr_ptr + 1];
                stack_push(&progr_stack, &to_push);
                instr_ptr += 2;
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
                instr_ptr += 1;
                break;
            }
            case OUT:
            {
                proc_val_t to_out = 0;
                stack_pop(&progr_stack, &to_out);
                printf("Processor out: %lld\n", to_out);
                instr_ptr += 1;
                break;
            }
            case DUMP:
            {
                STACK_DUMP(&progr_stack);
                instr_ptr += 1;
                break;
            }

            default:
                printf("Invalid instruction #%lld\n", code[instr_ptr]);
                is_valid_code = false;
                break;
        }
    }

    stack_dtor(&progr_stack);

    return 0;
}
