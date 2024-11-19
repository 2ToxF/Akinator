#ifndef _TREE_H
#define _TREE_H

#include "settings.h"

enum TreeError
{
    TREE_NO_ERR,
    TREE_INSERTION_BEFORE_ROOT_ERR,
    TREE_INSERT_BOTH_RELATIONS_ERR,
    TREE_NO_MEM_FOUND_ERR,
};

enum NodesRelation
{
    NO_RELATION,
    LEFT_SON,
    RIGHT_SON,
};

struct TreeNode_t
{
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


TreeError   TreeAddNode   (TreeNode_t* node, const TreeElem_t value, NodesRelation relation);
void        TreeDtor      (TreeNode_t* node);
bool        TreeFindLeaf  (TreeNode_t* node, NodesRelation relations_arr[], TreeElem_t value, int cur_level);
TreeNode_t* TreeInit      (const TreeElem_t value);
TreeError   TreeInsertNode(NodeInsertionData* ins_node_data);

#endif
