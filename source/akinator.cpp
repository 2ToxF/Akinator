#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "akinator.h"
#include "database.h"
#include "input_output.h"
#include "tree.h"
#include "utils.h"

static const char* const DATABASE_FILE_NAME = "database.txt";

struct NodeConnection
{
    TreeNode_t*   prev_node;
    TreeNode_t*   node;
    NodesRelation last_relation;
};

struct NewElemForDatabase
{
    char character[MAX_STR_LEN];
    char question [MAX_STR_LEN];
    int  correct_user_answer;
};

static CodeError RunGame   (TreeNode_t* db_tree);
static bool      RunOneGame(TreeNode_t* db_tree, CodeError* p_code_err);

static bool      AskOneMoreGame();
static void      AskQuestion   (NodeConnection* node_con);
static CodeError AskToSaveDB   (const char* database_fname, TreeNode_t* db_tree);
static void      GetNewData    (                          NewElemForDatabase* new_data);
static CodeError UpdateDatabase(NodeConnection* node_con, NewElemForDatabase* new_data);

// --------------------------------------------------------------------------------------------------------


CodeError RunAkinator()
{
    CodeError code_err = NO_ERROR;

    TreeNode_t* db_tree_root = NULL;
    if ((code_err = ReadDatabase(DATABASE_FILE_NAME, &db_tree_root)) != NO_ERROR)
        return code_err;

    if (db_tree_root == NULL)
        return TREE_NOT_INITED_ERR;

    TreeDump(db_tree_root);

            return code_err;
        }

        else if ((user_answer == 'n' || user_answer == 'N') && getchar() == '\n')
            printf(BLU "OK, bye! :) Have a nice day ^_^" WHT "\n");

        else
        {
            printf(RED "Sorry, I don't know such answer :( Please, write 'y', 'Y', 'n' or 'N'\n");
            ClearBuffer();
            continue;
        }

        break;
    }

    return NO_ERROR;
}


static void GetNewData(NewElemForDatabase* new_data)
{
    printf("\n" MAG "WARNING: don't use double quotes while writing answers - it definitely WILL cause errors" WHT "\n");
    printf(YEL "So, who (or maybe what) was your character? " WHT "\n");
    scanf("%[^\n]", new_data->character);
    ClearBuffer();

    while (true)
    {
        printf(YEL "Now please write the question that differs your character and character mentioned in my answer." WHT "\n");
        scanf("%[^\n]", new_data->question);
        ClearBuffer();

        if (new_data->question[strlen(new_data->question) - 1] == '?')
            break;
        printf(RED "Sorry, that is not a question :(" WHT "\n");
    }

    printf(YEL "And the last. What should user answer on your question if he wished for your character?" WHT "\n");
    /* last info (correct_user_answer) i need to get in the cycle in another function --> check UpdateDatabase() */
}


#define INSERT_NEW_DATA_(__caps_dir__)                                                                          \
    if (TreeInsertNode(&ins_node_data) != TREE_NO_ERROR)                                                        \
        return TREE_ERROR;                                                                                      \
                                                                                                                \
    if (node_con->last_relation == LEFT_SON)                                                                    \
    {                                                                                                           \
        if (TreeAddNode(node_con->prev_node->left, new_data->character, __caps_dir__##_SON) != TREE_NO_ERROR)   \
            return TREE_ERROR;                                                                                  \
    }                                                                                                           \
                                                                                                                \
    else if (node_con->last_relation == RIGHT_SON)                                                              \
    {                                                                                                           \
        if (TreeAddNode(node_con->prev_node->right, new_data->character, __caps_dir__##_SON) != TREE_NO_ERROR)  \
            return TREE_ERROR;                                                                                  \
    }

static CodeError UpdateDatabase(NodeConnection* node_con, NewElemForDatabase* new_data)
{
    NodeInsertionData ins_node_data = {node_con->prev_node, node_con->last_relation, node_con->node, ROOT, {}};
    strcpy(ins_node_data.ins_value, new_data->question);

    while (true)
    {
        new_data->correct_user_answer = getchar();

        if ((new_data->correct_user_answer == 'y' || new_data->correct_user_answer == 'Y') && getchar() == '\n')
        {
            ins_node_data.relation_with_next = LEFT_SON;
            INSERT_NEW_DATA_(RIGHT);
        }


        else if ((new_data->correct_user_answer == 'n' || new_data->correct_user_answer == 'N') && getchar() == '\n')
        {
            ins_node_data.relation_with_next = RIGHT_SON;
            INSERT_NEW_DATA_(LEFT);
        }

        else
        {
            printf(RED "Sorry, I don't know such answer :( Please, write 'y', 'Y', 'n' or 'N'" WHT "\n");
            ClearBuffer();
            continue;
        }

        break;
    }

    return NO_ERROR;
}

#undef INSERT_NEW_DATA_
