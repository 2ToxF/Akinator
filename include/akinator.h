#ifndef _AKINATOR_H
#define _AKINATOR_H

#include "settings.h"

#define CHECK_USER_ANSWER(__var_for_check__, __char1__, __char2__) \
    ((__var_for_check__ == __char1__ || __var_for_check__ == __char2__) && getchar() == '\n')

CodeError RunAkinator();

#endif
