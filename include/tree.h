#ifndef _TREE_H
#define _TREE_H

#include "settings.h"

enum TreeError
{
    TREE_NO_ERROR,
    TREE_INSERTION_BEFORE_ROOT_ERR,
    TREE_INSERT_BOTH_RELATIONS_ERR,
    TREE_NO_MEM_FOUND_ERR,
};

enum NodesRelation
{
    LEFT_SON,
    RIGHT_SON,
    ROOT,
};

// enum DataKind
// {
//     QUESTION,
//     ANSWER,
// };

struct TreeNode_t
{
    // DataKind    kind_of_data;
    TreeElem_t  data;
    TreeNode_t* left;
    TreeNode_t* right;
};

struct NodeInsertionData
{
    TreeNode_t*      prev_node;
    NodesRelation    relation_with_prev;

    TreeNode_t*      cur_node;
    NodesRelation    relation_with_next;

    TreeElem_t ins_value;
};

TreeError   TreeAddNode   (TreeNode_t* node, const TreeElem_t value,
                           NodesRelation relation /*, DataKind kind_of_new_data*/);
void        TreeDtor      (TreeNode_t* node);
TreeNode_t* TreeInit      (const TreeElem_t value /*, DataKind kind_of_root_data*/);
TreeError   TreeInsertNode(NodeInsertionData* ins_node_data);

void        DumpClose     ();
void        TreeDump      (TreeNode_t* node);

#endif
