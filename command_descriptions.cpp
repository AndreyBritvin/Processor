#define MAKE_ARITHMETICAL_OPERATION(OPERATION)          \
{                                                       \
    proc_val_t  first_mul = 0;                          \
    proc_val_t second_mul = 0;                          \
                                                        \
    stack_pop(&proc.stack, &second_mul);                \
    stack_pop(&proc.stack,  &first_mul);                \
                                                        \
    proc_val_t to_mul = first_mul OPERATION second_mul; \
    stack_push(&proc.stack, &to_mul);                   \
                                                        \
    proc.instr_ptr += 1;                                \
}

#define MAKE_UNAR_OPERATION(FUNC)                       \
{                                                       \
    proc_val_t stack_value = 0;                         \
    stack_pop(&proc.stack, &stack_value);               \
                                                        \
    stack_value = FUNC(stack_value);                    \
    stack_push(&proc.stack, &stack_value);              \
                                                        \
    proc.instr_ptr += 1;                                \
}

#define JUMP_CONDITION(CONDITION)                                      \
{                                                                      \
    proc_val_t  first_val = 0;                                         \
    proc_val_t second_val = 0;                                         \
    stack_pop(&proc.stack, &second_val);                               \
    stack_pop(&proc.stack, &first_val);                                \
                                                                       \
    if (first_val CONDITION second_val)                                \
    {                                                                  \
        proc.instr_ptr = (uint64_t) proc.code.arr[proc.instr_ptr + 1]; \
    }                                                                  \
    else                                                               \
    {                                                                  \
        proc.instr_ptr += 2;                                           \
    }                                                                  \
}

CMD_DESCR_DEF(HLT, "hlt", NO_ARGUMENTS,
{
    is_valid_code = false;
}
)

//////////////////////////////////////////////

CMD_DESCR_DEF(PUSH, "push", REG_IMM_ARG,
{
    proc_val_t to_push = *get_arg(&proc);
    stack_push(&proc.stack, &to_push);
}
)

/////////////////////////////////////////////

CMD_DESCR_DEF(ADD, "add", NO_ARGUMENTS,
{
    MAKE_ARITHMETICAL_OPERATION(+);
}
)

///////////////////////////////////////////////

CMD_DESCR_DEF(OUT, "out", NO_ARGUMENTS,
{
    proc_val_t to_out = 0;
    stack_pop(&proc.stack, &to_out);
    printf("Processor out: %lg\n", to_out);
    proc.instr_ptr += 1;
}
)

///////////////////////////////////////////////

CMD_DESCR_DEF(DUMP, "dump", NO_ARGUMENTS,
{
    // STACK_DUMP(&progr_stack);
    processor_dump(proc);
    proc.instr_ptr += 1;
}
)

///////////////////////////////////////////////

CMD_DESCR_DEF(POP, "pop", REG_IMM_ARG,
{
    proc_val_t pop_val = 0;
    stack_pop(&proc.stack, &pop_val);
    proc_val_t* addr_to_change = get_arg(&proc);
    *addr_to_change = pop_val;
}
)

//////////////////////////////////////////////

CMD_DESCR_DEF(SUB, "sub", NO_ARGUMENTS,
{
    MAKE_ARITHMETICAL_OPERATION(-);
}
)

//////////////////////////////////////////////

CMD_DESCR_DEF(MUL, "mul", NO_ARGUMENTS,
{
    MAKE_ARITHMETICAL_OPERATION(*);
}
)

//////////////////////////////////////////////

CMD_DESCR_DEF(DIV, "div", NO_ARGUMENTS,
{
    MAKE_ARITHMETICAL_OPERATION(/);
}
)

////////////////////////////////////////////

CMD_DESCR_DEF(IN, "in", NO_ARGUMENTS,
{
    proc_val_t user_input = 0;
    printf("Enter a number: ");
    scanf("%lf", &user_input);

    stack_push(&proc.stack, &user_input);
    proc.instr_ptr += 1;
}
)

///////////////////////////////////////////

CMD_DESCR_DEF(SIN, "sin", NO_ARGUMENTS,
{
    MAKE_UNAR_OPERATION(sin);
}
)

///////////////////////////////////////////

CMD_DESCR_DEF(COS, "cos", NO_ARGUMENTS,
{
   MAKE_UNAR_OPERATION(cos);
}
)

//////////////////////////////////////////

CMD_DESCR_DEF(SQRT, "sqrt", NO_ARGUMENTS,
{
    MAKE_UNAR_OPERATION(sqrt);
}
)

/////////////////////////////////////////////

CMD_DESCR_DEF(JMP, "jump", LABEL_ARG,
{
    proc.instr_ptr = (uint64_t) proc.code.arr[proc.instr_ptr + 1];
}
)

///////////////////////////////////////////////////////

CMD_DESCR_DEF(JA, "ja", LABEL_ARG,
{
    JUMP_CONDITION(>);
}
)

///////////////////////////////////////////////////////

CMD_DESCR_DEF(JAE, "jae", LABEL_ARG,
{
    JUMP_CONDITION(>=);
}
)

///////////////////////////////////////////////////////

CMD_DESCR_DEF(JB, "jb", LABEL_ARG,
{
    JUMP_CONDITION(<);
}
)

///////////////////////////////////////////////////////

CMD_DESCR_DEF(JBE, "jbe", LABEL_ARG,
{
    JUMP_CONDITION(<=);
}
)

///////////////////////////////////////////////////////

CMD_DESCR_DEF(JE, "je", LABEL_ARG,
{
    JUMP_CONDITION(==);
}
)

///////////////////////////////////////////////////////

CMD_DESCR_DEF(JNE, "jne", LABEL_ARG,
{
    JUMP_CONDITION(!=);
}
)

/////////////////////////////////////////////////////////////

CMD_DESCR_DEF(GPU, "gpu", NO_ARGUMENTS,
{
    print_RAM(proc);
    proc.instr_ptr += 1;
}
)


///////////////////////////////////////////////////////////////

CMD_DESCR_DEF(CALL, "call", LABEL_ARG,
{
    // proc_val_t ip_proc_val = (proc_val_t)proc.instr_ptr;
    // print_ret_val_stack(proc.ret_val_stack);
    stack_push(&proc.ret_val_stack, &proc.instr_ptr);
    proc.instr_ptr = (uint64_t) proc.code.arr[proc.instr_ptr + 1];
}
)

////////////////////////////////////////////////////////////

CMD_DESCR_DEF(RET, "ret", NO_ARGUMENTS,
{
    stack_pop(&proc.ret_val_stack, &proc.instr_ptr);
    // print_ret_val_stack(proc.ret_val_stack);
    proc.instr_ptr += 2;
}
)

///////////////////////////////////////////////////////////

CMD_DESCR_DEF(FLOOR, "flr", NO_ARGUMENTS,
{
    MAKE_UNAR_OPERATION(floor);
}
)

///////////////////////////////////////////////////////////

CMD_DESCR_DEF(GULAG, "glg", NO_ARGUMENTS,
{
    proc_val_t to_pop = 0;
    stack_pop(&proc.stack, &to_pop);
}
)

///////////////////////////////////////////////////////////

CMD_DESCR_DEF(MEOW, "meow", NO_ARGUMENTS,
{
    printf("Meow (at addr == %lu)\n", proc.instr_ptr++);
}
)

#undef PUT_ONE_CMD
#undef MAKE_UNAR_OPERATION
#undef MAKE_ARITHMETICAL_OPERATION
#undef JUMP_CONDITION
