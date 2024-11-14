#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input_output.h"
#include "tree.h"

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// ---------------------------------------------------------------------------------------------------------------
// ------> !!! TREE PART !!! <-----------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


TreeError TreeAddNode(TreeNode_t* node, const TreeElem_t value,
                      NodesRelation relation /*, DataKind kind_of_new_data*/)
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
    // node->kind_of_data = kind_of_new_data;

    if (relation == LEFT_SON)
        prev_node->left = node;
    else
        prev_node->right = node;

    return TREE_NO_ERROR;
}


void TreeDtor(TreeNode_t* node)
{
    if (node->left != NULL)
        TreeDtor(node->left);
    if (node->right != NULL)
        TreeDtor(node->right);
    free(node);
}

TreeNode_t* TreeInit(const TreeElem_t value /*, DataKind kind_of_root_data*/)
{
    TreeNode_t* tree_root = (TreeNode_t*) calloc(1, sizeof(TreeNode_t));
    if (tree_root == NULL)
        return tree_root;

    strcpy(tree_root->data, value);
    // tree_root->kind_of_data = kind_of_root_data;
    return tree_root;
}


TreeError TreeInsertNode(NodeInsertionData* ins_node_data)
{
    TreeError tree_err = TREE_NO_ERROR;

    if (ins_node_data->prev_node == NULL)
        return TREE_INSERTION_BEFORE_ROOT_ERR;

    if (ins_node_data->relation_with_prev == LEFT_SON)
        ins_node_data->prev_node->left = NULL;
    else if (ins_node_data->relation_with_prev == RIGHT_SON)
        ins_node_data->prev_node->right = NULL;
    else
        return TREE_INSERT_BOTH_RELATIONS_ERR;

    if ((tree_err = TreeAddNode(ins_node_data->prev_node, ins_node_data->ins_value,
                                ins_node_data->relation_with_prev)) != TREE_NO_ERROR)
        return tree_err;

    // TODO: можно сделать красивее и быстрее, если поместить left и right в массив
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


// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// ---------------------------------------------------------------------------------------------------------------
// ------> !!! DUMP PART !!! <-----------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


#define DUMP_LOG_PATH  "logs/"
#define DUMP_DOT_FNAME DUMP_LOG_PATH "dump_dotfile"

static const int MAX_CMD_LEN             = 100;
static const int MAX_FNAME_LEN           = 30;
static const char* const DUMP_HTML_FNAME = DUMP_LOG_PATH "tree_log.html";

static char dump_graph_fname[MAX_FNAME_LEN] = {};

static int   dump_number    = 0;
static FILE* dump_html_fptr = NULL;

static void DumpDotFile  (TreeNode_t* node);
static void DumpDotNode  (FILE* dot_file, TreeNode_t* node, TreeNode_t* prev_node, NodesRelation tree_relation);
static void DumpHtmlFile ();
static void SystemCallDot();

void DumpClose()
{
    if (dump_html_fptr != NULL)
    {
        fclose(dump_html_fptr);
        dump_html_fptr = NULL;
    }

    dump_number = 0;
}

static void DumpDotFile(TreeNode_t* node)
{
    FILE* dot_file = fopen(DUMP_DOT_FNAME, "w");

    fprintf(dot_file,
            "digraph {\n"
            "\tranksep=\"0.7\"\n"
            "\tnslimit=1;\n"
            "\tnslimit1=1;\n"
            "\trankdir=TB;\n"
            "\tsplines=false;\n"
            "\tbgcolor=\"darkslategray1\";\n"
            "\tfontname=\"Courier New\";\n"
            "\tnode[shape=\"Mrecord\",color=\"coral\",style=\"filled\",fillcolor=\"darkmagenta\",fontcolor=\"white\",fontsize=14];\n"
            "\tedge[color=\"blue\"];\n"
            );
    DumpDotNode(dot_file, node, NULL, ROOT);
    fprintf(dot_file, "}\n");

    fclose(dot_file); dot_file = NULL;
}


static void DumpDotNode(FILE* dot_file, TreeNode_t* node, TreeNode_t* prev_node, NodesRelation tree_relation)
{
    if (node == NULL)
        return;

    const char* str_left_ptr = NULL;
    const char* str_right_ptr = NULL;

    if (node->left == NULL)
        str_left_ptr = "-";
    else
    {
        str_left_ptr = "no";
        DumpDotNode(dot_file, node->left, node, LEFT_SON);
    }

    if (node->right == NULL)
        str_right_ptr = "-";
    else
    {
        str_right_ptr = "yes";
        DumpDotNode(dot_file, node->right, node, RIGHT_SON);
    }

    fprintf(dot_file, "\tp%p[label=\"{data: %s | {<left> %s | <right> %s}}\"];\n",
            node, node->data, str_left_ptr, str_right_ptr);

    if (prev_node != NULL)
    {
        if (tree_relation == LEFT_SON)
            fprintf(dot_file, "\tp%p:left:s -> p%p;\n", prev_node, node);
        else if (tree_relation == RIGHT_SON)
            fprintf(dot_file, "\tp%p:right:s -> p%p;\n", prev_node, node);
    }
}


static void DumpHtmlFile()
{
    if (dump_number == 0)
        dump_html_fptr = fopen(DUMP_HTML_FNAME, "w");

    fprintf(dump_html_fptr,
            "<img src=\"%s\"> <br>\n"
            "------------------------------------------------------------------------------------------------------------------------------------------- <br><br>\n"
            "\n",
            dump_graph_fname);
}


void TreeDump(TreeNode_t* node)
{
    DumpDotFile(node);
    SystemCallDot();
    DumpHtmlFile();
    ++dump_number;
}


void SystemCallDot()
{
    sprintf(dump_graph_fname, "tree_graph%d.svg", dump_number);

    char command[MAX_CMD_LEN] = {};
    sprintf(command, "dot -Tsvg " DUMP_DOT_FNAME " -o " DUMP_LOG_PATH "%s", dump_graph_fname);

    if (system(command) != 0)
    {
        printf(RED "ERROR: SYSTEM CALL ERROR" BLU " (command = %s)" WHT "\n", command);
        return;
    }
}

#undef DUMP_LOG_PATH
#undef DUMP_DOT_FNAME
