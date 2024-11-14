#include <stdio.h>
#include <stdlib.h>

#include "database.h"
#include "input_output.h"
#include "utils.h"

static char* start_database_buf = NULL;

static void SaveNode(FILE* database, TreeNode_t* node, int node_level);
static CodeError ScanNode(char** database_buf, TreeNode_t** node, int recursion_level);


CodeError ReadDatabase(const char* database_fname, TreeNode_t** root)
{
    CodeError code_err = NO_ERROR;

    if (*root != NULL)
        return TREE_ALREADY_INITED_ERR;

    char*    database_buf = NULL;
    long int database_buf_length = 0;
    if ((code_err = MyFread(&database_buf, &database_buf_length, database_fname)) != NO_ERROR)
        return code_err;

    start_database_buf = database_buf;

    return ScanNode(&database_buf, root, 0);
}


#define PRINT_TABS_                       \
    for (int i = 0; i < node_level; ++i)  \
        fputc('\t', database)

static void SaveNode(FILE* database, TreeNode_t* node, int node_level)
{
    PRINT_TABS_;
    fprintf(database, "{\n");
    PRINT_TABS_;
    fprintf(database, "\t%s\n", node->data);

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


#define READ_SENTENCE_(__var_name__)                  \
    BufNextSentence(database_buf);                    \
    char __var_name__[MAX_STR_LEN] = {};              \
    sscanf(*database_buf, "%[^\r\n]", __var_name__);  \
    BufNextSentence(database_buf)

static CodeError ScanNode(char** database_buf, TreeNode_t** node, int recursion_level)
{
    CodeError code_err = NO_ERROR;

    if (*node == NULL)
    {
        READ_SENTENCE_(root_data);
        *node = TreeInit(root_data);
// TreeDump(node);
        if (*node == NULL)
            return NO_MEM_FOR_TREE_ERR;
        return ScanNode(database_buf, node, recursion_level);
    }
printf("!!!!!! before \"left\"  (r = %d): shift: %llu; char: %d = %c\n",
       recursion_level, *database_buf - start_database_buf, **database_buf, **database_buf);
    if (**database_buf == '{')
    {
        READ_SENTENCE_(left_son_data);
        if (TreeAddNode(*node, left_son_data, LEFT_SON) != TREE_NO_ERROR)
            return TREE_ERROR;
// TreeDump(node);
        if ((code_err = ScanNode(database_buf, &(*node)->left, recursion_level + 1)) != NO_ERROR)
            return code_err;
    }
    else if (**database_buf == '*')
        BufNextSentence(database_buf);
    else
        return DATABASE_READ_LEFT_ERR;

printf("!!!!!! before \"right\" (r = %d): shift: %llu; char: %d = %c\n",
       recursion_level, *database_buf - start_database_buf, **database_buf, **database_buf);
    BufSkipSpaces(database_buf);
    if (**database_buf == '{')
    {
        READ_SENTENCE_(right_son_data);
        if (TreeAddNode(*node, right_son_data, RIGHT_SON) != TREE_NO_ERROR)
            return TREE_ERROR;
// TreeDump(node);
        if ((code_err = ScanNode(database_buf, &(*node)->right, recursion_level + 1)) != NO_ERROR)
            return code_err;
    }
    else if (**database_buf == '*')
        BufNextSentence(database_buf);
    else
        return DATABASE_READ_RIGHT_ERR;

// printf("!!!!!! before '}'     (r = %d): shift: %llu; char: %d = %c\n",
       // recursion_level, *database_buf - start_database_buf, **database_buf, **database_buf);
    if (**database_buf != '}')
        return DATABASE_READ_END_ERR;
    BufNextString(database_buf);
//TreeDump(node);
    return NO_ERROR;
}

#undef READ_SENTENCE_


CodeError SaveTreeData(const char* database_fname, TreeNode_t* root)
{
    if (root == NULL)
        return NULL_TREE_TO_SAVE_ERR;

    FILE* database = fopen(database_fname, "w");
    if (database == NULL)
        return FILE_NOT_OPENED_ERR;

    SaveNode(database, root, 0);

    fclose(database); database = NULL;
    return NO_ERROR;
}
