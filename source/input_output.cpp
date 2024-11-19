#include <stdio.h>
#include <stdlib.h>

#include "input_output.h"
#include "utils.h"


CodeError MyFread(char** input_buffer, long int* input_buffer_length, const char* input_file_name)
{
    CodeError code_err = NO_ERR;

    FILE* input_fptr = fopen(input_file_name, "rb");
    if (input_fptr == NULL)
        return FILE_NOT_OPENED_ERR;

    if ((code_err = Fsize(input_file_name, input_buffer_length)) != NO_ERR)
        return code_err;

    *input_buffer = (char*) calloc(*input_buffer_length, sizeof(char));
    long long unsigned int success_read_string_length = fread(*input_buffer, 1, *input_buffer_length - 1, input_fptr);
    if (success_read_string_length != (long long unsigned int) (*input_buffer_length - 1))
        return WRONG_BUFSIZE_ERR;

    fclose(input_fptr); input_fptr = NULL;
    return code_err;
}


void PrintCodeError(CodeError code_error)
{
    #define ERR_DESCR_(error)                                   \
        case (error):                                           \
            printf("\n\n" RED "CODE_ERROR: " #error WHT "\n");  \
            break

    switch(code_error)
    {
        case NO_ERR:
            printf("\n\n" GRN "Code was completed without errors" WHT "\n");
            break;

        ERR_DESCR_(FILE_NOT_OPENED_ERR);
        ERR_DESCR_(FILLING_FSTAT_ERR);
        ERR_DESCR_(WRONG_BUFSIZE_ERR);

        ERR_DESCR_(SYSTEM_CALL_ERR);

        ERR_DESCR_(DATABASE_READ_END_ERR);
        ERR_DESCR_(DATABASE_READ_LEFT_ERR);
        ERR_DESCR_(DATABASE_READ_RIGHT_ERR);

        ERR_DESCR_(NO_MEM_FOR_TREE_ERR);
        ERR_DESCR_(TREE_NOT_INITED_ERR);

        ERR_DESCR_(NULL_TREE_TO_SAVE_ERR);
        ERR_DESCR_(TREE_ALREADY_INITED_ERR);
        ERR_DESCR_(TREE_ERROR);

        ERR_DESCR_(UNKNOWN_AKINATOR_MODE_ERR);

        default:
            printf("\n\n" RED "CODE_ERROR: UNKNOWN_ERROR" WHT "\n");
            break;
    }

    #undef ERR_DESCR_
}
