COMMAND_DESCR(HLT, "hlt",
{
    fprintf(output_file, "%d\n", HLT);
},
    is_valid_code = false;
)

//////////////////////////////////////////////

COMMAND_DESCR(PUSH, "push",
{
    proc_val_t to_scan = 0;
    fscanf(input_file, "%d", &to_scan);
    fprintf(output_file, "%d %d\n", PUSH, to_scan);
},
    proc_val_t to_push = proc.code[proc.instr_ptr + 1];
    stack_push(&proc.stack, &to_push);
    proc.instr_ptr += 2;
)

/////////////////////////////////////////////

COMMAND_DESCR(ADD, "add",
{
    fprintf(output_file, "%d\n", ADD);
},
    proc_val_t  first_add = 0;
    proc_val_t second_add = 0;
    stack_pop(&proc.stack, &second_add);
    stack_pop(&proc.stack,  &first_add);
    proc_val_t to_add = first_add + second_add;
    stack_push(&proc.stack, &to_add);
    proc.instr_ptr += 1;
)

///////////////////////////////////////////////

COMMAND_DESCR(OUT, "out",
{
    fprintf(output_file, "%d\n", OUT);
},
    proc_val_t to_out = 0;
    stack_pop(&proc.stack, &to_out);
    printf("Processor out: %lld\n", to_out);
    proc.instr_ptr += 1;
)

///////////////////////////////////////////////

COMMAND_DESCR(DUMP, "dump",
{
    fprintf(output_file, "%d\n", DUMP);
},
    // STACK_DUMP(&progr_stack);
    processor_dump(proc);
    proc.instr_ptr += 1;
)

///////////////////////////////////////////////

