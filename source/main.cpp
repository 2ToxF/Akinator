#include <stdio.h>
#include <stdlib.h>

#include "tree.h"

int main()
{
    TreeNode_t* root = TreeInit(50);
    if (root == NULL)
        return 1;
    TreeDump(root);

    TreeAddNode(root, 30);
    TreeDump(root);
    TreeAddNode(root, 10);
    TreeDump(root);
    TreeAddNode(root, 70);
    TreeDump(root);
    TreeAddNode(root, 65);
    TreeDump(root);
    TreeAddNode(root, 80);
    TreeDump(root);
    TreeAddNode(root, 66);
    TreeDump(root);
    TreeAddNode(root, 5);
    TreeDump(root);
    TreeAddNode(root, 11);
    TreeDump(root);
    TreeAddNode(root, 64);
    TreeDump(root);
    TreeAddNode(root, 75);
    TreeDump(root);
    TreeAddNode(root, 90);
    TreeDump(root);

    TreeDtor(root);
    DumpClose();

    return 0;
}
