COMMAND_DESCR(HLT, "hlt",
{
    fprintf(output_file, "%d\n", HLT);
    commands_counter += 1;
},
{
    is_valid_code = false;
}
)

//////////////////////////////////////////////

COMMAND_DESCR(PUSH, "push",
{
    proc_val_t to_scan = 0;
    fscanf(input_file, "%d", &to_scan);
    fprintf(output_file, "%d %d\n", PUSH, to_scan);
    commands_counter += 2;
},
{
    switch (proc.code.arr[proc.instr_ptr] & ~CMD_MASK)
    {
        case IMMEDIATE_VALUE << 5:
        {
            proc_val_t to_push = proc.code.arr[proc.instr_ptr + 1];
            stack_push(&proc.stack, &to_push);
            proc.instr_ptr += 2;
            break;
        }
        case REGISTER_VALUE << 5:
        {
            proc_val_t reg_num = proc.code.arr[proc.instr_ptr + 1];
            stack_push(&proc.stack, &proc.registers_arr[reg_num]);
            proc.instr_ptr += 2;
            break;
        }
        default:
        {
            fprintf(stderr, "No such command: %lx", (unsigned long) proc.code.arr[proc.instr_ptr]);
            break;
        }
    }
}
)

/////////////////////////////////////////////

COMMAND_DESCR(ADD, "add",
{
    fprintf(output_file, "%d\n", ADD);
    commands_counter += 1;
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
    fprintf(output_file, "%d\n", OUT);
    commands_counter += 1;
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
    fprintf(output_file, "%d\n", DUMP);
    commands_counter += 1;
},
{
    // STACK_DUMP(&progr_stack);
    processor_dump(proc);
    proc.instr_ptr += 1;
}
)

///////////////////////////////////////////////

