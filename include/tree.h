#ifndef _TREE_H
#define _TREE_H

#include "settings.h"

enum TreeError
{
    TREE_NO_ERROR,
    TREE_NO_MEM_ERR,
};

struct TreeNode_t;

TreeError   TreeAddNode (TreeNode_t* node, TreeElem_t value);
void        TreeDtor    (TreeNode_t* node);
void        TreeDump    (TreeNode_t* node);
TreeNode_t* TreeInit    (TreeElem_t value);

#endif
