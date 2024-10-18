#include "my_assembler.h"
#include "utils.h"
#include "my_processor.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

int compile_file(const char *input_filename, const char *output_filename)
{
    SAFE_OPEN_FILE( input_file,  input_filename, "r");
    SAFE_OPEN_FILE(output_file, output_filename, "w");

    fprintf(output_file, "       "); // commands counter
    fprintf(output_file, "%s %d\n", SIGNATURE, CODE_VER);

    char command[MAX_COMMAND_LEN] = "";
    int commands_counter = 0;

    while (fscanf(input_file, "%s", command) != EOF)
    {
        if(false)
        {
            // skip for next else if
        }
        #define COMMAND_DESCR(ENUM_NAME, STR_NAME, ASS_CODE, ...) \
            else if (strcmp(command, STR_NAME) == 0)              \
                ASS_CODE
            #include "../../command_descriptions.cpp"
        #undef COMMAND_DESCR
        /*
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
        */
    }

    fseek(output_file, 0, SEEK_SET);
    fprintf(output_file, "%d", commands_counter);

    fclose( input_file);
    fclose(output_file);

    return 0;
}
