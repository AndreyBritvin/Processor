#ifndef MY_ASSEMBLER_H_
#define MY_ASSEMBLER_H_

#include <stdlib.h>

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

int compile_file(const char *input_filename, const char *output_filename);
int fill_fixup(fixup_t *fixup_arr, size_t cmd_counter);
int find_label(label_t *label_arr, char *label_to_find);
int print_label_arr(label_t *label_arr);

#endif // MY_ASSEMBLER_H_
