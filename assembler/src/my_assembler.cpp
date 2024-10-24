#include "my_assembler.h"
#include "utils.h"
#include "my_processor.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

err_code_t compile_file(const char *input_filename, const char *output_filename)
{
    SAFE_OPEN_FILE( input_file,  input_filename, "r" );
    SAFE_OPEN_FILE(output_file, output_filename, "w" );

    char *bin_output_name = change_txt_name_to_bin(output_filename, strlen(output_filename));
    // printf("New name is '%s'\n", bin_output_name);
    SAFE_OPEN_FILE(output_file_bin, bin_output_name, "wb");
    free(bin_output_name);

    signature_t signature = {0, *((uint64_t *)SIGNATURE), CODE_VER, 0};

    fprintf(output_file, "       "); // commands counter
    fprintf(output_file, "%s %lu\n", SIGNATURE, CODE_VER);

    char command[MAX_COMMAND_LEN] = "";
    size_t  commands_counter = 0;

    proc_val_t proc_code[10] = {};

    static label_t labels[MAX_LABEL_COUNT] = {};
    static size_t first_free_label = 0;

    // fixup_t fixup [MAX_FIXUP_COUNT] = {};

    while (fscanf(input_file, "%s", command) != EOF)
    {
        if (strchr(command, ':') != NULL)
        {
            printf("At addres %lu we met up with label %s\n", commands_counter, command);

            if (find_label(labels, command) >= first_free_label)
            {
                labels[first_free_label].label_code_ptr = commands_counter;
                strcpy(labels[first_free_label++].label_str, command);
            }
        }

        #define COMMAND_DESCR(ENUM_NAME, STR_NAME, ARG_TYPE, ...)                           \
            else if (strcmp(command, STR_NAME) == 0)                                        \
            {                                                                               \
                if (ARG_TYPE == NO_ARGUMENTS)                                               \
                {                                                                           \
                    fprintf(output_file, "%d\n", ENUM_NAME);                                \
                    commands_counter += 1;                                                  \
                }                                                                           \
                else if (ARG_TYPE == REG_IMM_ARG)                                           \
                {                                                                           \
                    parse_argument(input_file, output_file, &commands_counter, ENUM_NAME);  \
                    commands_counter += 2;                                                  \
                }                                                                           \
                else if (ARG_TYPE == LABEL_ARG)                                             \
                {                                                                           \
                    fill_jump_arg(input_file, output_file, labels, ENUM_NAME);              \
                    commands_counter += 2;                                                  \
                }                                                                           \
            }

            #include "../../command_descriptions.cpp"
        #undef COMMAND_DESCR

        else
        {
            fprintf(stderr, "Unknown command %s\n", command); // TODO this part dont work...
            assert(0);
        }
    }

    signature.code_size = commands_counter;
    fwrite(&signature, sizeof(signature), 1,output_file_bin);

    fseek(output_file, 0, SEEK_SET);
    fprintf(output_file, "%lu", commands_counter);

    fclose( input_file);
    fclose(output_file);
    fclose(output_file_bin);

    return OK;
}

char *change_txt_name_to_bin(const char *txt_filename, size_t filename_len) // TODO:      \/
{
    char *bin_filename = (char *) calloc(filename_len + 4 + 1, sizeof(char)); //4 = bin/ TODO: redo this to auto
    printf("Calloced addr %p\n", bin_filename);
    memcpy(bin_filename + 4, txt_filename, filename_len);
    printf("text in new addr %s\n", bin_filename);
    bin_filename[0] = 'b';
    bin_filename[1] = 'i';
    bin_filename[2] = 'n';
    bin_filename[3] = '/';

    bin_filename[filename_len + 1] = 'b';
    bin_filename[filename_len + 2] = 'i';
    bin_filename[filename_len + 3] = 'n';

    return bin_filename;
}

size_t find_label(label_t *label_arr, char *label_to_find)
{
    size_t label_ind = 0;
    for (; label_ind < MAX_LABEL_COUNT; label_ind++)
    {
        if (strcmp(label_to_find, label_arr[label_ind].label_str) == 0)
        {
            break;
        }
    }

    return label_ind;
}

err_code_t print_label_arr(label_t *label_arr)
{
    printf("Begin printing labels table:\n");
    for (size_t label_ind = 0; label_ind < MAX_LABEL_COUNT; label_ind++)
    {
        printf("%02lu: addr: %lu, name: %s\n", label_ind, label_arr[label_ind].label_code_ptr,
                                                          label_arr[label_ind].label_str);
    }

    return OK;
}

err_code_t fill_jump_arg(FILE *input_file, FILE *output_file, label_t *labels, int cmd_type)
{
    proc_val_t to_scan = 0;
    char label_str[MAX_LABEL_LEN] = {};
    char jump_arg [MAX_COMMAND_LEN] = {};

    fgets(jump_arg, MAX_COMMAND_LEN, input_file);
    printf("Readed value is '%s'", jump_arg);

    if (sscanf(jump_arg, "%lld", &to_scan) == 1)
    {
        fprintf(output_file, "%d %lld\n", cmd_type, to_scan);
        printf("We are in immediate_jump value\n");
    }
    else if (sscanf(jump_arg, " %s:", label_str) == 1)
    {
        fprintf(output_file, "%d %lu\n", cmd_type, labels[find_label(labels, label_str)].label_code_ptr);
        // print_label_arr(labels);
    }
    else
    {
        printf("Wtf this is command: %s", jump_arg);
    }

    return OK;
}

err_code_t parse_argument(FILE *input_file, FILE *output_file, size_t *commands_counter, int command)
{
    proc_val_t to_scan    = 0;
    char register_num     = 0;
    // char register_name[5] = {}; // register name e.g. ax, bx
    char push_arg[MAX_COMMAND_LEN] = {};

    fgets(push_arg, MAX_COMMAND_LEN, input_file);
    // printf("Readed value is '%s'\n", push_arg);
    // printf("Command in input = %d\n", command);

    if (strchr(push_arg, '[') != NULL)
    {
        command |= RAM_VALUE;
    }

    if (strchr(push_arg, '+') != NULL)
    {
        command |=  REGISTER_VALUE;
        command |= IMMEDIATE_VALUE;

        *commands_counter += 1;
        (command & RAM_VALUE) ? sscanf(push_arg,  "[%cx + %lld]", &register_num, &to_scan):
                                sscanf(push_arg,  " %cx + %lld" , &register_num, &to_scan);
    }
    else if (strchr(push_arg, 'x') != NULL)
    {
        command |=  REGISTER_VALUE;

        (command & RAM_VALUE) ? sscanf(push_arg, " [%cx", &register_num):
                                sscanf(push_arg,  " %cx", &register_num);

        // printf("Register_num is %c (%d)\n", register_num, register_num);
    }
    else
    {
        command |=  IMMEDIATE_VALUE;

        (command & RAM_VALUE) ? sscanf(push_arg, " [%lld", &to_scan):
                                sscanf(push_arg,  " %lld", &to_scan);
    }
    register_num -= 'a';

    fprintf(output_file, "%d", command);

    if (command & REGISTER_VALUE)
    {
        fprintf(output_file, " %d", register_num);
    }
    if (command & IMMEDIATE_VALUE)
    {
        fprintf(output_file, " %lld", to_scan);
    }
    fprintf(output_file, "\n");
    // printf("Command in OUTput = %d\n", command);

    return OK;
}

err_code_t print_code_to_file(FILE *output_file_txt, /*FILE *output_file_bin,*/ const char *format ...)
{
    va_list fprintf_args; // TODO: do it via array
    va_start(fprintf_args, format);
    vfprintf(output_file_txt, format, fprintf_args);
    // fwrite();
    // vfprintf(output_file_bin, format, fprintf_args);
    va_end(fprintf_args);

    return OK;
}
