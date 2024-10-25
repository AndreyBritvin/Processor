#ifndef UTILS_H_
#define UTILS_H_

#include <errno.h>

#define SAFE_OPEN_FILE(file_ptr, filename, mode)                                   \
                        FILE *file_ptr = fopen(filename, mode);                    \
                        if (file_ptr == NULL)                                      \
                        {                                                          \
                            printf("Unable open output file '%s'\n", filename);    \
                            return ERROR_FILE;                                     \
                        }

// header utils
typedef int err_code_t;

enum errors
{
    OK,
    ERROR_FILE,
    ERROR_ALLOC,
    ERROR_LABEL_NOT_FOUND,
    ERROR_UNKNOWN_COMMAND,
};

#endif //UTILS_H_
