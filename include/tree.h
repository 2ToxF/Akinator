#ifndef _TREE_H
#define _TREE_H

#include "settings.h"

#define BLK "\033[0;30m"
#define RED "\033[0;31m"
#define GRN "\033[0;32m"
#define YEL "\033[0;33m"
#define BLU "\033[0;34m"
#define MAG "\033[0;35m"
#define CYN "\033[0;36m"
#define WHT "\033[0;37m"

enum TreeError
{
    TREE_NO_ERROR,
    TREE_NO_MEM_ERR,
};

struct TreeNode_t;

void        DumpClose   ();
TreeError   TreeAddNode (TreeNode_t* node, TreeElem_t value);
void        TreeDtor    (TreeNode_t* node);
void        TreeDump    (TreeNode_t* node);
TreeNode_t* TreeInit    (TreeElem_t value);

#endif
