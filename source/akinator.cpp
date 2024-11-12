#include <stdio.h>
#include <stdlib.h>

#include "akinator.h"
#include "database.h"
#include "tree.h"

static const char* const DATABASE_FILE_NAME = "database.txt";


CodeError RunAkinator()
{
    CodeError code_err = NO_ERROR;

    TreeNode_t* root = TreeInit("Is your character an animal?", QUESTION);
    if (root == NULL)
        return NO_MEM_FOR_TREE_ERR;
    // TreeDump(root);

    TreeAddNode(root, "Poltorashka", RIGHT_SON, ANSWER);
    // TreeDump(root);
    TreeAddNode(root, "Is your character a maths teacher?", LEFT_SON, QUESTION);
    // TreeDump(root);
    TreeAddNode(root->left, "Kalinichenko", RIGHT_SON, ANSWER);
    // TreeDump(root);
    TreeAddNode(root, "Is your character a physics teacher?", LEFT_SON, QUESTION);
    // TreeDump(root);
    TreeAddNode(root->left->left, "DED CHTOLE?", LEFT_SON, QUESTION);
    // TreeDump(root);
    TreeAddNode(root->left->left->left, "DED INSIDE", LEFT_SON, ANSWER);
    // TreeDump(root);
    TreeAddNode(root->left->left, "I don't know such(", RIGHT_SON, ANSWER);
    TreeDump(root);

    SaveTreeData(DATABASE_FILE_NAME, root);

    TreeDtor(root);
    DumpClose();

    return code_err;
}
