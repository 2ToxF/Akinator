#ifndef _TREE_H
#define _TREE_H

#include "settings.h"

enum TreeError
{
    TREE_NO_ERROR,
    TREE_NO_MEM_ERR,
};

enum TreeRelation
{
    ROOT,
    LEFT_SON,
    RIGHT_SON,
};

enum DataKind
{
    QUESTION,
    ANSWER,
};

struct TreeNode_t
{
    DataKind    kind_of_data;
    TreeElem_t  data;
    TreeNode_t* left;
    TreeNode_t* right;
};

TreeError   TreeAddNode (TreeNode_t* node, const TreeElem_t value,
                         TreeRelation relation, DataKind kind_of_new_data);
void        TreeDtor    (TreeNode_t* node);
TreeNode_t* TreeInit    (const TreeElem_t value, DataKind kind_of_root_data);

void        DumpClose   ();
void        TreeDump    (TreeNode_t* node);

#endif
