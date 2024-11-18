#ifndef _TREE_DUMP_H
#define _TREE_DUMP_H

#include "settings.h"

#define DUMP_LOG_PATH  "logs/"
#define DUMP_DOT_FNAME DUMP_LOG_PATH "dump_dotfile"
#define DUMP_HTML_FNAME DUMP_LOG_PATH "tree_log.html"

void DumpClose();
void TreeDump (TreeNode_t* node);

#endif
