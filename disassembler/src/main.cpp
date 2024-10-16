#include "my_disassembler.h"
#include <stdio.h>

int main(const int argc, const char **argv)
{
    switch (argc)
    {
        case 1:
            decompile_file("../program_codes/test.txt", "../compiled/comp_test.txt");
            break;
        case 2:
            decompile_file(argv[1], "../compiled/comp_test.txt");
            break;
        case 3:
            decompile_file(argv[1], argv[2]);
            break;
        default:
            printf("Wrong arguments count. %s [input_file] [out_file]\n", argv[0]);
            break;
    }

    return 0;
}
