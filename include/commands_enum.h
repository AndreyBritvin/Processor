#ifndef COMMANDS_ENUM_H_
#define COMMANDS_ENUM_H_

enum processor_commands
{
    #define COMMAND_DESCR(ENUM_NAME, STR_NAME, ASS_CODE, ...) ENUM_NAME,

    #include "../command_descriptions.cpp"
    #undef COMMAND_DESCR
    LAST_COMMAND_DONT_USE_IT
    // HLT      =  0,
    // PUSH     =  1,
    // ADD      =  2,
    // SUB      =  3,
    // MUL      =  4,
    // DIV      =  5,
    // OUT      =  6,
    // IN       =  7,
    // SQRT     =  8,
    // SIN      =  9,
    // COS      = 10,
    // DUMP     = 11,
    // POP      = 12,
    // JMP      = 13,
    // JA       = 14,
    // GPU      = 15,
    // CALL     = 16,
    // RET      = 17,
};

#endif // COMMANDS_ENUM_H_
