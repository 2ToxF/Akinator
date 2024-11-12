#include <stdio.h>

#include "database.h"

static void SaveNode(FILE* database, TreeNode_t* node, int node_level);


CodeError ReadDatabase(const char* /*database_fname*/, TreeNode_t* /*root*/)
{
    return NO_ERROR;
}


#define PRINT_TABS_                       \
    for (int i = 0; i < node_level; ++i)  \
        fputc('\t', database)

static void SaveNode(FILE* database, TreeNode_t* node, int node_level)
{
    PRINT_TABS_;
    fprintf(database, "{\n");
    PRINT_TABS_;
    fprintf(database, "%s\n", node->data);

    if (node->left != NULL)
        SaveNode(database, node->left, node_level + 1);
    else
    {
        PRINT_TABS_;
        fprintf(database, "\t*\n");
    }

    if (node->right != NULL)
        SaveNode(database, node->right, node_level + 1);
    else
    {
        PRINT_TABS_;
        fprintf(database, "\t*\n");
    }

    PRINT_TABS_;
    fprintf(database, "}\n");
}

#undef PRINT_TABS_


CodeError SaveTreeData(const char* database_fname, TreeNode_t* root)
{
    if (root == NULL)
        return NULL_TREE_TO_SAVE_ERR;

    FILE* database = fopen(database_fname, "w");
    if (database == NULL)
        return FILE_NOT_OPENED_ERR;

    SaveNode(database, root, 0);
    return NO_ERROR;
}
