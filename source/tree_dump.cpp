#include <stdio.h>
#include <stdlib.h>

#include "input_output.h"
#include "tree.h"
#include "tree_dump.h"

static const int MAX_CMD_LEN   = 100;
static const int MAX_FNAME_LEN = 30;

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
    DumpDotNode(dot_file, node, NULL, NO_RELATION);
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

    fprintf(dot_file, "\tp%p[label=\"{%s | {<left> %s | <right> %s}}\"];\n",
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
    if (dump_html_fptr == NULL)
    {
        dump_html_fptr = fopen(DUMP_HTML_FNAME, "w");
        if (dump_html_fptr == NULL)
            printf(RED "ERROR during dump: FILE WAS NOT OPENED (name of file: %s)" WHT "\n", DUMP_HTML_FNAME);
    }

    fprintf(dump_html_fptr,
            "<img src=\"%s\"> <br>\n"
            "-----------------------------------------------------------------------------------------------"
            "-----------------------------------------------------------------------------------------------"
            "------------------------------------------------------------------------------------ <br><br>\n"
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
        printf(RED "ERROR during dump: SYSTEM CALL ERROR" BLU " (command = %s)" WHT "\n", command);
        return;
    }
}
