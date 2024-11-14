#include <stdio.h>
#include <stdlib.h>

#include "akinator.h"
#include "database.h"
#include "tree.h"

static const char* const DATABASE_FILE_NAME = "database.txt";


CodeError RunAkinator()
{
    CodeError code_err = NO_ERROR;

    TreeNode_t* root = TreeInit("Is your character an animal?");
    if (root == NULL)
        return NO_MEM_FOR_TREE_ERR;
    // TreeDump(root);

    TreeAddNode(root, "Cute kitty?", RIGHT_SON);
    // TreeDump(root);
    TreeAddNode(root->right, "Poltorashka", RIGHT_SON);
    // TreeDump(root);
    TreeAddNode(root->right, "DED KVADROBER CHTOLE?", LEFT_SON);
    // TreeDump(root);
    TreeAddNode(root->right->left, "DED KVADROBER", RIGHT_SON);
    // TreeDump(root);
    TreeAddNode(root->right->left, "I don't know such(", LEFT_SON);
    // TreeDump(root);
    TreeAddNode(root, "Is your character a maths teacher?", LEFT_SON);
    // TreeDump(root);
    TreeAddNode(root->left, "Kalinichenko", RIGHT_SON);
    // TreeDump(root);
    TreeAddNode(root, "Is your character a physics teacher?", LEFT_SON);
    // TreeDump(root);
    TreeAddNode(root->left->left, "DED CHTOLE?", LEFT_SON);
    // TreeDump(root);
    TreeAddNode(root->left->left->left, "DED INSIDE", LEFT_SON);
    // TreeDump(root);
    TreeAddNode(root->left->left, "I don't know such(", RIGHT_SON);
    TreeDump(root);

    if ((code_err = SaveTreeData(DATABASE_FILE_NAME, root)) != NO_ERROR)
        return code_err;

    TreeDtor(root); root = NULL;
    if ((code_err = ReadDatabase(DATABASE_FILE_NAME, &root)) != NO_ERROR)
    {
        TreeDump(root);
        printf("root_ptr = %p", root);
        return code_err;
    }
    TreeDump(root);

    DumpClose();

    return code_err;
}
