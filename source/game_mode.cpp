#include <stdio.h>
#include <string.h>

#include "akinator.h"
#include "database.h"
#include "game_mode.h"
#include "input_output.h"
#include "tree.h"
#include "utils.h"

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
};

static bool      AskOneMoreGame();
static void      AskQuestion   (NodeConnection* node_connection);
static void      GetNewData    (NewElemForDatabase* new_data);
static bool      GiveAnswer    (NodeConnection* node_connection, CodeError* p_code_err);
static bool      RunOneGame    (TreeNode_t* db_tree, CodeError* p_code_err);
static CodeError UpdateDatabase(NodeConnection* node_connection, NewElemForDatabase* new_data);


static bool AskOneMoreGame()
{
    int user_answer = 0;
    while (true)
    {
        if (user_answer != '\n' && user_answer != 0)
            ClearBuffer();

        printf(YEL "Do you wanna play one more time? (Y|N)" WHT "\n");
        user_answer = getchar();

        if (CHECK_USER_ANSWER(user_answer, 'y', 'Y'))
            return true;

        else if (CHECK_USER_ANSWER(user_answer, 'n', 'Y'))
            return false;

        else
        {
            printf(RED "Sorry, I don't know such answer :( Please, write 'y', 'Y', 'n' or 'N'\n");
            continue;
        }

        break;
    }
}


static void AskQuestion(NodeConnection* node_connection)
{
    printf(YEL "Your character %s? (Y|N)" WHT "\n", node_connection->node->data);
    int user_answer = getchar();

    if (CHECK_USER_ANSWER(user_answer, 'y', 'Y'))
    {
        node_connection->prev_node = node_connection->node;
        node_connection->node = node_connection->node->right;
        node_connection->last_relation = RIGHT_SON;
    }

    else if (CHECK_USER_ANSWER(user_answer, 'n', 'N'))
    {
        node_connection->prev_node = node_connection->node;
        node_connection->node = node_connection->node->left;
        node_connection->last_relation = LEFT_SON;
    }

    else
    {
        printf(RED "Sorry, I don't know such answer :( Please, write 'y', 'Y', 'n' or 'N'\n");
        if (user_answer != '\n')
            ClearBuffer();
    }
}


static void GetNewData(NewElemForDatabase* new_data)
{
    printf("\n" MAG "!!! WARNING: don't use double quotes while writing answers - it definitely WILL cause errors !!!" WHT "\n\n");

    while (true)
    {
        printf(CYN "So, who (or maybe what) was your character? " WHT "\n");
        scanf("%[^\n]", new_data->character);

        if (new_data->character[0] != '\0')
        {
            ClearBuffer();
            break;
        }

        printf(RED "You can't enter empty name of object" WHT "\n");
        ClearBuffer();
    }

    while (true)
    {
        printf(CYN "What is the differecnce between your character and my answer.\n"
                    "Your character (continue the phrase) " WHT);
        scanf("%[^\n]", new_data->question);

        if (new_data->question[0] != '\0')
        {
            ClearBuffer();
            break;
        }

        printf(RED "You can't enter empty feature of object" WHT "\n");
        ClearBuffer();
    }
}


CodeError RunGameMode(TreeNode_t* db_tree)
{
    CodeError code_err = NO_ERROR;

    while (RunOneGame(db_tree, &code_err)) {}

    if (code_err != NO_ERROR)
        return code_err;

    return NO_ERROR;
}


static bool GiveAnswer(NodeConnection* node_connection, CodeError* p_code_err)
{
    int user_answer = 0;

    while (true)
    {
        if (user_answer != '\n' && user_answer != 0)
            ClearBuffer();

        printf(CYN "I think your character is \"%s\". Is that correct? (Y|N)" WHT "\n", node_connection->node->data);
        user_answer = getchar();

        if (CHECK_USER_ANSWER(user_answer, 'y', 'Y'))
            printf(YEL "Thanks for playing then!" WHT "\n");

        else if (CHECK_USER_ANSWER(user_answer, 'n', 'N'))
        {
            NewElemForDatabase new_data = {};
            GetNewData(&new_data);
            if ((*p_code_err = UpdateDatabase(node_connection, &new_data)) != NO_ERROR)
                return false;
        }

        else
        {
            printf(RED "Sorry, I don't know such answer :( Please, write 'y', 'Y', 'n' or 'N'\n");
            continue;
        }

        return true;
    }
}


static bool RunOneGame(TreeNode_t* db_tree, CodeError* p_code_err)
{
    NodeConnection node_connection = {db_tree, db_tree, NO_RELATION};

    while (true)
    {
        if (node_connection.node == NULL)
        {
            printf(YEL "Sorry, I don't know such character :(" WHT "\n");
            return true;
        }

        else if (node_connection.node->left == NULL && node_connection.node->right == NULL)
        {
            if (!GiveAnswer(&node_connection, p_code_err))
                return false;
            break;
        }

        AskQuestion(&node_connection);
    }

    return AskOneMoreGame();
}


static CodeError UpdateDatabase(NodeConnection* node_connection, NewElemForDatabase* new_data)
{
    NodeInsertionData ins_node_data = {node_connection->prev_node, node_connection->last_relation, node_connection->node, NO_RELATION, {}};
    strcpy(ins_node_data.ins_value, new_data->question);

    ins_node_data.relation_with_next = LEFT_SON;
    if (TreeInsertNode(&ins_node_data) != TREE_NO_ERROR)
        return TREE_ERROR;

    if (node_connection->last_relation == LEFT_SON)
    {
        if (TreeAddNode(node_connection->prev_node->left, new_data->character, RIGHT_SON) != TREE_NO_ERROR)
            return TREE_ERROR;
    }

    else if (node_connection->last_relation == RIGHT_SON)
    {
        if (TreeAddNode(node_connection->prev_node->right, new_data->character, RIGHT_SON) != TREE_NO_ERROR)
            return TREE_ERROR;
    }
    return NO_ERROR;
}
