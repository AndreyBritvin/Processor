#ifndef MY_ASSEMBLER_H_
#define MY_ASSEMBLER_H_

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <assert.h>
#include "utils.h"
#include "commands_enum.h"

static const int MAX_LABEL_COUNT = 32;
static const int MAX_LABEL_LEN   = 32;
static const int MAX_FIXUP_COUNT = 32;

enum arg_types
{
    NO_ARGUMENTS = 0,
    REG_IMM_ARG  = 1,
    LABEL_ARG    = 2,
};

struct fixup_t
{
    size_t pos_to_fixup;
    size_t label_to_fixup;
};

struct label_t
{
    char      label_str[MAX_LABEL_LEN];
    size_t              label_code_ptr;
    size_t                  label_size;
};

struct signature_t
{
    uint64_t code_size;
    uint64_t signature;
    uint64_t   version;
    uint64_t   nothing;
};

err_code_t compile_file(const char *input_filename, const char *output_filename);
size_t find_label(label_t *label_arr, char *label_to_find);
err_code_t print_label_arr(label_t *label_arr);
err_code_t fill_jump_arg(FILE *input_file, FILE *output_file, label_t *labels, int cmd_type);
err_code_t parse_argument(FILE *input_file, FILE *output_file, size_t *commands_counter, int command);

err_code_t print_code_to_file(FILE *output_file_txt, /*FILE *output_file_bin,*/ const char *format ...);

char *change_txt_name_to_bin(const char *txt_filename, size_t filename_len);

#endif // MY_ASSEMBLER_H_
