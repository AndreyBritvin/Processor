#include <stdio.h>
#include <string.h>
#include "my_stack.h"
#include "my_processor.h"

int main(const int argc, const char **argv)
{
    // proc_val_t proga_arr[20] = {PUSH, 30, PUSH, 70, DUMP, ADD, OUT, HLT};
    proc_code_t proga = {};

    if (argc == 1)
    {
        printf("You should use filename after programm call: %s exapmple_asm.txt", argv[0]);
    }

    if (read_code(argv[1], &proga) == OK)
    {
        run_code(proga);
    }
    else
    {
        printf("\nSomething got wrong in reading code... Errno = %s\n", strerror(errno));
    }

    return 0;
}
