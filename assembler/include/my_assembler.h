#ifndef MY_ASSEMBLER_H_
#define MY_ASSEMBLER_H_

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>

static const int MAX_LABEL_COUNT = 32;
static const int MAX_LABEL_LEN   = 32;
static const int MAX_FIXUP_COUNT = 32;

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

int compile_file(const char *input_filename, const char *output_filename);
int fill_fixup(fixup_t *fixup_arr, size_t cmd_counter);
int find_label(label_t *label_arr, char *label_to_find);
int print_label_arr(label_t *label_arr);
int fill_jump_arg(FILE *input_file, FILE *output_file, label_t *labels, int cmd_type);
int parse_argument(FILE *input_file, FILE *output_file, int *commands_counter, int command);

int print_code_to_file(FILE *output_file_txt, /*FILE *output_file_bin,*/ const char *format ...);

char *change_txt_name_to_bin(const char *txt_filename, size_t filename_len);

#endif // MY_ASSEMBLER_H_
