#ifndef _DATABASE_H
#define _DATABASE_H

#include "tree.h"

CodeError ReadDatabase(const char* database_fname, TreeNode_t* root);
CodeError SaveTreeData(const char* database_fname, TreeNode_t* root);

#endif
