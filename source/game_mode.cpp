#define TX_USE_SPEAK
#include <TXLib.h>

#include <stdio.h>
#include <string.h>

#include "akinator.h"
#include "database.h"
#include "game_mode.h"
#include "input_output.h"
#include "tree.h"
#include "utils.h"

static char  speak_buffer[MAX_SPEAK_BUFFER_LEN] = {};
static char* cur_speak = speak_buffer;

struct NodeContext
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
static void      AskQuestion   (NodeContext* node_context);
static void      GetNewData    (NewElemForDatabase* new_data);
static bool      GiveAnswer    (NodeContext* node_context, CodeError* p_code_err);
static bool      RunOneGame    (TreeNode_t* db_tree, CodeError* p_code_err);
static CodeError UpdateDatabase(NodeContext* node_context, NewElemForDatabase* new_data);


static bool AskOneMoreGame()
{


    int user_answer = 0;
    while (true)
    {
        if (user_answer != '\n' && user_answer != 0)
            ClearBuffer();

        PRINT_SPEAK_COLOR_MES(YEL, "Do you wanna play one more time? (Y|N)\n");
        user_answer = getchar();

        if (CHECK_USER_ANSWER(user_answer, 'y', 'Y'))
            return true;

        else if (CHECK_USER_ANSWER(user_answer, 'n', 'Y'))
            return false;

        else
        {
            PRINT_SPEAK_COLOR_MES(RED, "Sorry, I don't know such answer :( Please, write 'y', 'Y', 'n' or 'N'\n");
            continue;
        }

        break;
    }
}


static void AskQuestion(NodeContext* node_context)
{


    PRINT_SPEAK_COLOR_MES(YEL, "Your character %s? (Y|N)\n", node_context->node->data);

    int user_answer = getchar();

    if (CHECK_USER_ANSWER(user_answer, 'y', 'Y'))
    {
        node_context->prev_node = node_context->node;
        node_context->node = node_context->node->right;
        node_context->last_relation = RIGHT_SON;
    }

    else if (CHECK_USER_ANSWER(user_answer, 'n', 'N'))
    {
        node_context->prev_node = node_context->node;
        node_context->node = node_context->node->left;
        node_context->last_relation = LEFT_SON;
    }

    else
    {
        PRINT_SPEAK_COLOR_MES(RED, "Sorry, I don't know such answer :( Please, write 'y', 'Y', 'n' or 'N'\n");
        if (user_answer != '\n')
            ClearBuffer();
    }
}


static void GetNewData(NewElemForDatabase* new_data)
{


    PRINT_SPEAK_COLOR_MES(MAG, "\n!!! WARNING: don't use double quotes while writing answers - "
                               "it definitely WILL cause errors !!!\n\n");

    while (true)
    {
        PRINT_SPEAK_COLOR_MES(CYN, "So, who (or maybe what) was your character?\n");
        scanf("%[^\n]", new_data->character);

        if (new_data->character[0] != '\0')
        {
            ClearBuffer();
            break;
        }

        PRINT_SPEAK_COLOR_MES(RED, "You can't enter empty name of object\n");
        ClearBuffer();
    }

    while (true)
    {
        PRINT_SPEAK_COLOR_MES(CYN, "What is the differecnce between your character and my answer.\n"
                             "Your character (continue the phrase) ");
        scanf("%[^\n]", new_data->question);

        if (new_data->question[0] != '\0')
        {
            ClearBuffer();
            break;
        }

        PRINT_SPEAK_COLOR_MES(RED, "You can't enter empty feature of object\n");
        ClearBuffer();
    }
}


CodeError RunGameMode(TreeNode_t* db_tree)
{
    CodeError code_err = NO_ERR;

    while (RunOneGame(db_tree, &code_err)) {}

    if (code_err != NO_ERR)
        return code_err;

    return NO_ERR;
}


static bool GiveAnswer(NodeContext* node_context, CodeError* p_code_err)
{


    int user_answer = 0;

    while (true)
    {
        if (user_answer != '\n' && user_answer != 0)
            ClearBuffer();

        PRINT_SPEAK_COLOR_MES(CYN, "I think your character is \"%s\". Is that correct? (Y|N)\n",
                              node_context->node->data);
        user_answer = getchar();

        if (CHECK_USER_ANSWER(user_answer, 'y', 'Y'))
        {
            PRINT_SPEAK_COLOR_MES(YEL, "Thanks for playing then!\n");
        }

        else if (CHECK_USER_ANSWER(user_answer, 'n', 'N'))
        {
            NewElemForDatabase new_data = {};
            GetNewData(&new_data);
            if ((*p_code_err = UpdateDatabase(node_context, &new_data)) != NO_ERR)
                return false;
        }

        else
        {
            PRINT_SPEAK_COLOR_MES(RED, "Sorry, I don't know such answer :( Please, write 'y', 'Y', 'n' or 'N'\n");
            continue;
        }

        return true;
    }
}


static bool RunOneGame(TreeNode_t* db_tree, CodeError* p_code_err)
{


    NodeContext node_context = {db_tree, db_tree, NO_RELATION};

    while (true)
    {
        if (node_context.node == NULL)
        {
            PRINT_SPEAK_COLOR_MES(YEL, "Sorry, I don't know such character :(\n");
            return true;
        }

        else if (node_context.node->left == NULL && node_context.node->right == NULL)
        {
            if (!GiveAnswer(&node_context, p_code_err))
                return false;
            break;
        }

        AskQuestion(&node_context);
    }

    return AskOneMoreGame();
}


static CodeError UpdateDatabase(NodeContext* node_context, NewElemForDatabase* new_data)
{
    NodeInsertionData ins_node_data = {node_context->prev_node, node_context->last_relation,
                                       node_context->node, NO_RELATION, {}};
    strcpy(ins_node_data.ins_value, new_data->question);

    ins_node_data.relation_with_next = LEFT_SON;
    if (TreeInsertNode(&ins_node_data) != TREE_NO_ERR)
        return TREE_ERROR;

    if (node_context->last_relation == LEFT_SON)
    {
        if (TreeAddNode(node_context->prev_node->left, new_data->character, RIGHT_SON) != TREE_NO_ERR)
            return TREE_ERROR;
    }

    else if (node_context->last_relation == RIGHT_SON)
    {
        if (TreeAddNode(node_context->prev_node->right, new_data->character, RIGHT_SON) != TREE_NO_ERR)
            return TREE_ERROR;
    }
    return NO_ERR;
}
