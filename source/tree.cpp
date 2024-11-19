#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input_output.h"
#include "tree.h"


TreeError TreeAddNode(TreeNode_t* node, const TreeElem_t value, NodesRelation relation)
{
    TreeNode_t* prev_node = node;

    while (node != NULL)
    {
        prev_node = node;
        if (relation == LEFT_SON)
            node = node->left;
        else
            node = node->right;
    }

    node = (TreeNode_t*) calloc(1, sizeof(TreeNode_t));
    if (node == NULL)
        return TREE_NO_MEM_FOUND_ERR;

    strcpy(node->data, value);

    if (relation == LEFT_SON)
        prev_node->left = node;
    else
        prev_node->right = node;

    return TREE_NO_ERR;
}


void TreeDtor(TreeNode_t* node)
{
    if (node->left != NULL)
        TreeDtor(node->left);
    if (node->right != NULL)
        TreeDtor(node->right);
    free(node);
}


bool TreeFindLeaf(TreeNode_t* node, NodesRelation relations_arr[], TreeElem_t value, int cur_level)
{
    if (node == NULL)
        return false;

    if (node->left == NULL && node->right == NULL)
    {
        if (strcmp(node->data, value) == 0)
        {
            relations_arr[cur_level] = NO_RELATION;
            return true;
        }

        return false;
    }

    if (TreeFindLeaf(node->left, relations_arr, value, cur_level+1))
    {
        relations_arr[cur_level] = LEFT_SON;
        return true;
    }

    if (TreeFindLeaf(node->right, relations_arr, value, cur_level+1))
    {
        relations_arr[cur_level] = RIGHT_SON;
        return true;
    }

    return false;
}


TreeNode_t* TreeInit(const TreeElem_t value)
{
    TreeNode_t* tree_root = (TreeNode_t*) calloc(1, sizeof(TreeNode_t));
    if (tree_root == NULL)
        return tree_root;

    strcpy(tree_root->data, value);
    return tree_root;
}


TreeError TreeInsertNode(NodeInsertionData* ins_node_data)
{
    TreeError tree_err = TREE_NO_ERR;

    if (ins_node_data->prev_node == NULL)
        return TREE_INSERTION_BEFORE_ROOT_ERR;

    if (ins_node_data->relation_with_prev == LEFT_SON)
        ins_node_data->prev_node->left = NULL;
    else if (ins_node_data->relation_with_prev == RIGHT_SON)
        ins_node_data->prev_node->right = NULL;
    else
        return TREE_INSERT_BOTH_RELATIONS_ERR;

    if ((tree_err = TreeAddNode(ins_node_data->prev_node, ins_node_data->ins_value,
                                ins_node_data->relation_with_prev)) != TREE_NO_ERR)
        return tree_err;

    if (ins_node_data->relation_with_prev == LEFT_SON)
    {
        if (ins_node_data->relation_with_next == LEFT_SON)
            ins_node_data->prev_node->left->left = ins_node_data->cur_node;

        else if (ins_node_data->relation_with_next == RIGHT_SON)
            ins_node_data->prev_node->left->right = ins_node_data->cur_node;
    }

    else if (ins_node_data->relation_with_prev == RIGHT_SON)
    {
        if (ins_node_data->relation_with_next == LEFT_SON)
            ins_node_data->prev_node->right->left = ins_node_data->cur_node;

        else if (ins_node_data->relation_with_next == RIGHT_SON)
            ins_node_data->prev_node->right->right = ins_node_data->cur_node;
    }

    return tree_err;
}
