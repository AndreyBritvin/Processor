#include "my_stack.h"
#include "my_processor.h"

int main(const int argc, const char **argv)
{
    // proc_code_t proga[] = {PUSH, 30, PUSH, 70, DUMP, ADD, OUT, HLT};
    proc_code_t proga[20] = {}; // TODO: make auto size

    read_code("compiled/comp_test.txt", proga);

    run_code(proga);

    if (argc == 1)
    {

    }

    return 0;
}
