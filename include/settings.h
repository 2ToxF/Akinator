#ifndef _SETTINGS_H
#define _SETTINGS_H

// #define TREE_ELEM_T STRING

#if defined TREE_ELEM_T && TREE_ELEM_T == STRING
    typedef const char* TreeElem_t;
#else
    typedef double TreeElem_t;
#endif

#endif
