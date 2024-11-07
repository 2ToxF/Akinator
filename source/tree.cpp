#include <stdio.h>
#include <stdlib.h>

#include "tree.h"

struct TreeNode_t
{
    TreeElem_t  data;
    TreeNode_t* left;
    TreeNode_t* right;
};


#if defined TREE_ELEM_T && TREE_ELEM_T == STRING
#else
    TreeError TreeAddNode(TreeNode_t* node, TreeElem_t value)
    {
        TreeNode_t* prev_node = node;

        while (node != NULL)
        {
            prev_node = node;
            if (value < node->data)
                node = node->left;
            else
                node = node->right;
        }

        node = (TreeNode_t*) calloc(1, sizeof(TreeNode_t));
        if (node == NULL)
            return TREE_NO_MEM_ERR;
        node->data = value;

        if (value < prev_node->data)
            prev_node->left = node;
        else
            prev_node->right = node;

        return TREE_NO_ERROR;
    }


    void TreeDump(TreeNode_t* node)
    {
        putchar('(');
        printf("%lg", node->data);

        if (node->left != NULL)
            TreeDump(node->left);
        else
            putchar('*');

        if (node->right != NULL)
            TreeDump(node->right);
        else
            putchar('*');

        putchar(')');
    }
#endif


void TreeDtor(TreeNode_t* node)
{
    if (node->left != NULL)
        TreeDtor(node->left);
    if (node->right != NULL)
        TreeDtor(node->right);
    free(node);
}

TreeNode_t* TreeInit(TreeElem_t value)
{
    TreeNode_t* tree_root = (TreeNode_t*) calloc(1, sizeof(TreeNode_t));
    if (tree_root == NULL)
        return tree_root;

    tree_root->data = value;
    return tree_root;
}
