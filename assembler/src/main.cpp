#include "my_assembler.h"
#include <stdio.h>

int main(const int argc, const char **argv)
{
    label_t labels[MAX_LABEL_COUNT] = {};
    switch (argc)
    {
        case 1:
            compile_file("../program_codes/test.txt", "../compiled/comp_test.txt", labels, 0);
            return compile_file("../program_codes/test.txt", "../compiled/comp_test.txt", labels, 1);
            break;
        case 2:
            compile_file(argv[1], "../compiled/comp_test.txt", labels, 0);
            return compile_file(argv[1], "../compiled/comp_test.txt", labels, 1);
            break;
        case 3:
            compile_file(argv[1], argv[2], labels, 0);
            return compile_file(argv[1], argv[2], labels, 1);
            break;
        default:
            printf("Wrong arguments count. %s [input_file] [out_file]\n", argv[0]);
            break;
    }

    return 0;
}
