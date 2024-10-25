#define STOP_PROGRAMM() do{is_valid_code = false;} while(0)

#define DO_POP(variable)                                \
    proc_val_t variable = 0;                            \
    stack_pop(&proc.stack, &variable);                  \

#define DO_ARG_POP()                                    \
    DO_POP(pop_val)                                     \
    proc_val_t* addr_to_change = get_arg(&proc);        \
    *addr_to_change = pop_val;                          \

#define DO_PUSH(variable)                               \
    stack_push(&proc.stack, &variable);                 \

#define DO_ARG_PUSH()                                   \
    proc_val_t to_push = *get_arg(&proc);               \
    DO_PUSH(to_push);

#define INCREMENT_IP()                                  \
    proc.instr_ptr += 1;

#define GET_USER_VALUE(user_input)                      \
    proc_val_t user_input = 0;                          \
    printf("Enter a number: ");                         \
    scanf("%lf", &user_input);                          \

#define PRINT()                                         \
{                                                       \
    DO_POP(to_out);                                     \
    printf("Processor out: %lg\n", to_out);             \
    INCREMENT_IP();                                     \
}

#define MAKE_ARITHMETICAL_OPERATION(OPERATION)          \
{                                                       \
    DO_POP(second_mul);                                 \
    DO_POP( first_mul);                                 \
                                                        \
    proc_val_t to_mul = first_mul OPERATION second_mul; \
    DO_PUSH(to_mul)                                     \
                                                        \
    INCREMENT_IP();                                     \
}

#define MAKE_UNAR_OPERATION(FUNC)                       \
{                                                       \
    DO_POP(stack_value);                                \
    stack_value = FUNC(stack_value);                    \
    DO_PUSH(stack_value);                               \
                                                        \
    INCREMENT_IP();                                     \
}

#define JUMP_CONDITION(CONDITION)                                      \
{                                                                      \
    DO_POP(second_val);                                                \
    DO_POP( first_val);                                                \
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

//TODO: MAKE DSL define for funcs push, pop etc
//////////////////////////////////////////////

CMD_DESCR_DEF(HLT, "hlt", NO_ARGUMENTS,
{
    STOP_PROGRAMM();
}
)

//////////////////////////////////////////////

CMD_DESCR_DEF(PUSH, "push", REG_IMM_ARG,
{
    DO_ARG_PUSH();
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
    PRINT();
}
)

///////////////////////////////////////////////

CMD_DESCR_DEF(DUMP, "dump", NO_ARGUMENTS,
{
    processor_dump(proc);
    INCREMENT_IP();
}
)

///////////////////////////////////////////////

CMD_DESCR_DEF(POP, "pop", REG_IMM_ARG,
{
    DO_ARG_POP();
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
    GET_USER_VALUE(user_input);
    DO_PUSH(user_input)
    INCREMENT_IP();
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
    DO_POP(to_pop);
}
)

///////////////////////////////////////////////////////////

CMD_DESCR_DEF(MEOW, "meow", NO_ARGUMENTS,
{
    printf("Meow (at addr == %lu)\n", proc.instr_ptr++);
}
)

#undef STOP_PROGRAMM()

#undef DO_POP(variable)
#undef DO_ARG_POP()
#undef DO_PUSH(variable)
#undef DO_ARG_PUSH()
#undef INCREMENT_IP()
#undef GET_USER_VALUE(user_input)
#undef PRINT()

#undef MAKE_ARITHMETICAL_OPERATION(OPERATION)
#undef MAKE_UNAR_OPERATION(FUNC)
#undef JUMP_CONDITION(CONDITION)
