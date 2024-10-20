#define PUT_ONE_CMD(CMD) fprintf(output_file, "%d\n", CMD);\
                         commands_counter += 1;

#define MAKE_ARITHMETICAL_OPERATION(OPERATION)          \
    proc_val_t  first_mul = 0;                          \
    proc_val_t second_mul = 0;                          \
    stack_pop(&proc.stack, &second_mul);                \
    stack_pop(&proc.stack,  &first_mul);                \
    proc_val_t to_mul = first_mul OPERATION second_mul; \
    stack_push(&proc.stack, &to_mul);                   \
    proc.instr_ptr += 1;

#define MAKE_UNAR_OPERATION(FUNC)                       \
    proc_val_t stack_value = 0;                         \
    stack_pop(&proc.stack, &stack_value);               \
    stack_value = FUNC(stack_value);                    \
    stack_push(&proc.stack, &stack_value);              \
    proc.instr_ptr += 1;


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
    parse_argument(input_file, output_file, &commands_counter, PUSH);
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
    MAKE_ARITHMETICAL_OPERATION(+);
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
    parse_argument(input_file, output_file, &commands_counter, POP);
    commands_counter += 2;
},
{
    proc_val_t pop_val = 0;
    stack_pop(&proc.stack, &pop_val);
    proc_val_t* addr_to_change = get_arg_pop(&proc);
    *addr_to_change = pop_val;
}
)

//////////////////////////////////////////////

COMMAND_DESCR(SUB, "sub",
{
    PUT_ONE_CMD(SUB);
},
{
    MAKE_ARITHMETICAL_OPERATION(-);
}
)

//////////////////////////////////////////////

COMMAND_DESCR(MUL, "mul",
{
    PUT_ONE_CMD(MUL);
},
{
    MAKE_ARITHMETICAL_OPERATION(*);
}
)

//////////////////////////////////////////////

COMMAND_DESCR(DIV, "div",
{
    PUT_ONE_CMD(DIV);
},
{
    MAKE_ARITHMETICAL_OPERATION(/);
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
    MAKE_UNAR_OPERATION(sin);
}
)

///////////////////////////////////////////

COMMAND_DESCR(COS, "cos",
{
    PUT_ONE_CMD(COS);
},
{
   MAKE_UNAR_OPERATION(cos);
}
)

//////////////////////////////////////////

COMMAND_DESCR(SQRT, "sqrt",
{
    PUT_ONE_CMD(SQRT);
},
{
    MAKE_UNAR_OPERATION(sqrt);
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
#undef MAKE_UNAR_OPERATION
#undef MAKE_ARITHMETICAL_OPERATION
