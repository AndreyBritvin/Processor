#define PUT_ONE_CMD(CMD) fprintf(output_file, "%d\n", CMD);\
                         commands_counter += 1;

COMMAND_DESCR(HLT, "hlt",
{
    PUT_ONE_CMD(HLT)
},
{
    is_valid_code = false;
}
)

//////////////////////////////////////////////

COMMAND_DESCR(PUSH, "push",
{
    proc_val_t to_scan = 0;
    char register_num  = 0;
    char push_arg[MAX_COMMAND_LEN] = {};

    fgets(push_arg, MAX_COMMAND_LEN, input_file);
    printf("Readed value is '%s'\n", push_arg);

    if (sscanf(push_arg, "%d", &to_scan) == 1)
    {
        fprintf(output_file, "%d %d\n", PUSH | IMMEDIATE_VALUE, to_scan);
        // printf("We are in immediate_val\n");
    }
    else if (sscanf(push_arg, " %cx %d", &register_num, &to_scan) == 2)
    {
        fprintf(output_file, "%d %d %d\n", PUSH | REGISTER_VALUE | IMMEDIATE_VALUE,
                                           register_num - 'a', to_scan);
        commands_counter += 1;
        // printf("We are in register+immediate_value\n");
    }
    else if (sscanf(push_arg, " %cx", &register_num) == 1)
    {
        fprintf(output_file, "%d %d\n", PUSH | REGISTER_VALUE, register_num - 'a');
        // printf("We are in register_value\n");
    }
    else
    {
        printf("Wtf this is command: %s\n", push_arg);
    }

    commands_counter += 2;
},
{
    proc_val_t to_push = get_arg_push(&proc);
    stack_push(&proc.stack, &to_push);
}
)

/////////////////////////////////////////////

COMMAND_DESCR(ADD, "add",
{
    PUT_ONE_CMD(ADD);
},
{
    proc_val_t  first_add = 0;
    proc_val_t second_add = 0;
    stack_pop(&proc.stack, &second_add);
    stack_pop(&proc.stack,  &first_add);
    proc_val_t to_add = first_add + second_add;
    stack_push(&proc.stack, &to_add);
    proc.instr_ptr += 1;
}
)

///////////////////////////////////////////////

COMMAND_DESCR(OUT, "out",
{
    PUT_ONE_CMD(OUT);
},
{
    proc_val_t to_out = 0;
    stack_pop(&proc.stack, &to_out);
    printf("Processor out: %lld\n", to_out);
    proc.instr_ptr += 1;
}
)

///////////////////////////////////////////////

COMMAND_DESCR(DUMP, "dump",
{
    PUT_ONE_CMD(DUMP);
},
{
    // STACK_DUMP(&progr_stack);
    processor_dump(proc);
    proc.instr_ptr += 1;
}
)

///////////////////////////////////////////////

COMMAND_DESCR(POP, "pop",
{
    fprintf(output_file, "%d ", POP);
    char reg_index = 0;
    fscanf(input_file, " %c", &reg_index);
    fprintf(output_file, "%d\n", reg_index - 'a');
    commands_counter += 2;
},
{
    char reg_index = proc.code.arr[proc.instr_ptr + 1];
    proc_val_t pop_val = 0;
    stack_pop(&proc.stack, &pop_val);
    proc.registers_arr[reg_index] = pop_val;
    proc.instr_ptr += 2;
}
)

//////////////////////////////////////////////

COMMAND_DESCR(SUB, "sub",
{
    PUT_ONE_CMD(SUB);
},
{
    proc_val_t  first_sub = 0;
    proc_val_t second_sub = 0;
    stack_pop(&proc.stack, &second_sub);
    stack_pop(&proc.stack,  &first_sub);
    proc_val_t to_sub = first_sub - second_sub;
    stack_push(&proc.stack, &to_sub);
    proc.instr_ptr += 1;
}
)

//////////////////////////////////////////////

COMMAND_DESCR(MUL, "mul",
{
    PUT_ONE_CMD(MUL);
},
{
    proc_val_t  first_mul = 0;
    proc_val_t second_mul = 0;
    stack_pop(&proc.stack, &second_mul);
    stack_pop(&proc.stack,  &first_mul);
    proc_val_t to_mul = first_mul * second_mul;
    stack_push(&proc.stack, &to_mul);
    proc.instr_ptr += 1;
}
)

//////////////////////////////////////////////

COMMAND_DESCR(DIV, "div",
{
    PUT_ONE_CMD(DIV);
},
{
    proc_val_t  first_div = 0;
    proc_val_t second_div = 0;
    stack_pop(&proc.stack, &second_div);
    stack_pop(&proc.stack,  &first_div);
    proc_val_t to_div = first_div / second_div;
    stack_push(&proc.stack, &to_div);
    proc.instr_ptr += 1;
}
)

////////////////////////////////////////////

COMMAND_DESCR(IN, "in",
{
    PUT_ONE_CMD(IN);
},
{
    proc_val_t user_input = 0;
    printf("Enter a number: ");
    scanf("%d", &user_input);

    stack_push(&proc.stack, &user_input);
    proc.instr_ptr += 1;
}
)

///////////////////////////////////////////

COMMAND_DESCR(SIN, "sin",
{
    PUT_ONE_CMD(SIN);
},
{
    proc_val_t stack_value = 0;
    stack_pop(&proc.stack, &stack_value);
    stack_value = sin(stack_value);

    stack_push(&proc.stack, &stack_value);
    proc.instr_ptr += 1;
}
)

///////////////////////////////////////////

COMMAND_DESCR(COS, "cos",
{
    PUT_ONE_CMD(COS);
},
{
    proc_val_t stack_value = 0;
    stack_pop(&proc.stack, &stack_value);
    stack_value = cos(stack_value);

    stack_push(&proc.stack, &stack_value);
    proc.instr_ptr += 1;
}
)

//////////////////////////////////////////

COMMAND_DESCR(SQRT, "sqrt",
{
    PUT_ONE_CMD(SQRT);
},
{
    proc_val_t stack_value = 0;
    stack_pop(&proc.stack, &stack_value);
    stack_value = sqrt(stack_value);

    stack_push(&proc.stack, &stack_value);
    proc.instr_ptr += 1;
}
)

/////////////////////////////////////////////

COMMAND_DESCR(JMP, "jump",
{
    fill_jump_arg(input_file, output_file, labels, JMP);
    commands_counter += 2;
},
{
    proc.instr_ptr = proc.code.arr[proc.instr_ptr + 1];
}
)

///////////////////////////////////////////////////////

COMMAND_DESCR(JA, "ja",
{
    fill_jump_arg(input_file, output_file, labels, JA);
    commands_counter += 2;
},
{
    proc_val_t  first_val = 0;
    proc_val_t second_val = 0;
    stack_pop(&proc.stack, &second_val);
    stack_pop(&proc.stack, &first_val);

    if (first_val >= second_val)
    {
        proc.instr_ptr = proc.code.arr[proc.instr_ptr + 1];
    }
    else
    {
        proc.instr_ptr += 2;
    }
}
)

/////////////////////////////////////////////////////////////

COMMAND_DESCR(GPU, "gpu",
{
    PUT_ONE_CMD(GPU);
},
{
    print_RAM(proc);
    proc.instr_ptr += 1;
}
)


///////////////////////////////////////////////////////////////

COMMAND_DESCR(CALL, "call", //TODO remove copypaste
{
    fill_jump_arg(input_file, output_file, labels, CALL);
    commands_counter += 2;
},
{
    // proc_val_t ip_proc_val = (proc_val_t)proc.instr_ptr;
    print_ret_val_stack(proc.ret_val_stack);
    stack_push(&proc.ret_val_stack, &proc.instr_ptr);
    proc.instr_ptr = proc.code.arr[proc.instr_ptr + 1];
}
)

////////////////////////////////////////////////////////////

COMMAND_DESCR(RET, "ret",
{
    PUT_ONE_CMD(RET);
},
{
    stack_pop(&proc.ret_val_stack, &proc.instr_ptr);
    print_ret_val_stack(proc.ret_val_stack);
    proc.instr_ptr += 2;
}
)


#undef PUT_ONE_CMD
