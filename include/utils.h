#ifndef UTILS_H_
#define UTILS_H_

#include <errno.h>

#define SAFE_OPEN_FILE(file_ptr, filename, mode)                                   \
                        FILE *file_ptr = fopen(filename, mode);                    \
                        if (file_ptr == NULL)                                      \
                        {                                                          \
                            printf("Unable open output file '%s'\n", filename);    \
                            return errno;                                          \
                        }

// header utils

#endif //UTILS_H_
