#include <stdio.h>
#include <stdlib.h>

#include "tree.h"

int main()
{
    TreeNode_t* root = TreeInit(50);
    if (root == NULL)
        return 1;
    TreeDump(root); putchar('\n'); getchar();

    TreeAddNode(root, 30);
    TreeDump(root); putchar('\n'); getchar();
    TreeAddNode(root, 10);
    TreeDump(root); putchar('\n'); getchar();
    TreeAddNode(root, 70);
    TreeDump(root); putchar('\n'); getchar();
    TreeAddNode(root, 65);
    TreeDump(root); putchar('\n'); getchar();
    TreeAddNode(root, 80);
    TreeDump(root); putchar('\n'); getchar();
    TreeAddNode(root, 66);
    TreeDump(root); putchar('\n'); getchar();
    TreeDtor(root);

    return 0;
}
