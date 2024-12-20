#ifndef _INPUT_OUTPUT_H
#define _INPUT_OUTPUT_H

#include "settings.h"

#define BLK "\033[0;30m"
#define RED "\033[0;31m"
#define GRN "\033[0;32m"
#define YEL "\033[0;33m"
#define BLU "\033[0;34m"
#define MAG "\033[0;35m"
#define CYN "\033[0;36m"
#define WHT "\033[0;37m"

#define PRINT_SPEAK_COLOR_MES(__color__, __message__, ...)                                      \
    do {                                                                                        \
        char temp_speak_buffer[MAX_SPEAK_BUFFER_LEN] = {};                                      \
        char* temp_cur_speak = temp_speak_buffer;                                               \
                                                                                                \
        temp_cur_speak = temp_speak_buffer;                                                     \
        temp_cur_speak += sprintf(temp_cur_speak, __message__, ## __VA_ARGS__);                 \
                                                                                                \
        printf(__color__);                                                                      \
        PrintVoiceMessage(temp_speak_buffer);                                                   \
        printf(WHT);                                                                            \
    } while(0)

CodeError MyFread(char** input_buffer, long int* input_buffer_length, const char* input_file_name);
void PrintCodeError(CodeError code_err);
void PrintVoiceMessage(const char* message_buffer);

#endif
