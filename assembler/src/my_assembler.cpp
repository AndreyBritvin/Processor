#include "my_assembler.h"
#include "my_processor.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

int compile_file(const char *input_filename, const char *output_filename)
{
    FILE *input_file = fopen(input_filename, "r");
    if (input_file == NULL)
    {
        printf("Unable open input file '%s'\n", input_filename);
    }
    FILE *output_file = fopen(output_filename, "w");
    if (output_file == NULL)
    {
        printf("Unable open output file '%s'\n", output_filename);
    }

    char command[20] = "";
    while (fscanf(input_file, "%s", command) != EOF)
    {
        // printf("Command = %s", strerror(errno));
        if (strcmp(command, "push") == 0)
        {
            proc_val_t to_scan = 0;
            fscanf(input_file, "%d", &to_scan);
            fprintf(output_file, "%d %d\n", PUSH, to_scan);
        }
        else if (strcmp(command, "add") == 0)
        {
            fprintf(output_file, "%d\n", ADD);
        }
        else if (strcmp(command, "hlt") == 0)
        {
            fprintf(output_file, "%d\n", HLT);
        }
        else if (strcmp(command, "dump") == 0)
        {
            fprintf(output_file, "%d\n", DUMP);
        }
        else if (strcmp(command, "out") == 0)
        {
            printf("%d", fprintf(output_file, "%d\n", OUT));
        }
    }

    return 0;
}
