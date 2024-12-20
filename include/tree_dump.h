#ifndef _TREE_DUMP_H
#define _TREE_DUMP_H

#include "settings.h"

#define DUMP_LOG_PATH  "logs/"
#define DUMP_DOT_FILE_NAME DUMP_LOG_PATH "dump_dotfile"
const char* const DUMP_HTML_FILE_NAME = DUMP_LOG_PATH "tree_log.html";

void DumpClose();
void TreeDump (TreeNode_t* node);

#endif
