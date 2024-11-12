#include <stdio.h>

#include "input_output.h"

void PrintCodeError(CodeError code_error)
{
    #define ERR_DESCR_(error)                                   \
        case (error):                                           \
            printf("\n\n" RED "CODE_ERROR: " #error WHT "\n");  \
            break

    switch(code_error)
    {
        case NO_ERROR:
            printf("\n\n" GRN "Code was completed without errors" WHT "\n");
            break;

        ERR_DESCR_(FILE_NOT_OPENED_ERR);

        ERR_DESCR_(NO_MEM_FOR_TREE_ERR);
        ERR_DESCR_(NULL_TREE_TO_SAVE_ERR);
        ERR_DESCR_(TREE_ALREADY_INITED_ERR);

        default:
            printf("\n\n" RED "CODE_ERROR: UNKNOWN_ERROR" WHT "\n");
            break;
    }

    #undef ERR_DESCR_
}
