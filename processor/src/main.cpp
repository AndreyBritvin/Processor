#include "my_stack.h"
#include "my_processor.h"

int main(const int argc, const char **argv)
{
    // proc_val_t proga_arr[20] = {PUSH, 30, PUSH, 70, DUMP, ADD, OUT, HLT};
    proc_code_t proga = {};

    read_code(argv[1], &proga);

    run_code(proga);

    if (argc == 1)
    {

    }

    return 0;
}
