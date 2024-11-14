#include <stdio.h>
#include <sys/stat.h>

#include "utils.h"


void BufNextSentence(char** buffer)
{
    BufNextString(buffer);
    BufSkipSpaces(buffer);
}


void BufNextString(char** buffer)
{
    while (**buffer != '\n' && **buffer != '\0')
        ++(*buffer);
    ++(*buffer);
}


void BufSkipSpaces(char** buffer)
{
    while (**buffer == ' ' || **buffer == '\t' || **buffer == '\n' || **buffer == '\r')
        ++(*buffer);
}


CodeError Fsize(const char* file_name, long int* input_buffer_length)
{
    struct stat input_file_stat = {};
    if (stat(file_name, &input_file_stat) != 0)
        return FILLING_FSTAT_ERR;

    *input_buffer_length = input_file_stat.st_size + 1;
    return NO_ERROR;
}
