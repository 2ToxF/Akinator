#ifndef _UTILS_H
#define _UTILS_H

#include "settings.h"

void      BufNextSentence(char** buffer);
void      BufNextString  (char** buffer);
void      BufSkipSpaces  (char** buffer);
void      ClearBuffer    ();
CodeError GetFileSize    (const char* file_name, long int* input_buffer_length);

#endif
