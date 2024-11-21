#define TX_USE_SPEAK
#include <TXLib.h>

#include "akinator.h"
#include "input_output.h"
#include "tree.h"

int main()
{
    txDisableAutoPause();
    CodeError code_err = RunAkinator();
    PrintCodeError(code_err);
    return code_err;
}
