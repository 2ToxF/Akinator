#include "akinator.h"
#include "input_output.h"
#include "tree.h"

int main()
{
    CodeError code_err = RunAkinator();
    PrintCodeError(code_err);
    return code_err;
}
