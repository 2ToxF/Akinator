#ifndef _SETTINGS_H
#define _SETTINGS_H

static const int MAX_STR_LEN = 100;

typedef char TreeElem_t[MAX_STR_LEN];

enum CodeError
{
    NO_ERROR,

    FILE_NOT_OPENED_ERR,

    NO_MEM_FOR_TREE_ERR,
    NULL_TREE_TO_SAVE_ERR,
    TREE_ALREADY_INITED_ERR,
};

#endif
