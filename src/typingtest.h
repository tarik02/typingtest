#ifndef TT_TYPINGTEST_H
#define TT_TYPINGTEST_H

#include "platform.h"

#ifdef TT_UNIX
# ifndef TT_SIGNALS
#  define TT_SIGNALS 1
# endif
#endif

#define TT_COLOR_BLACK 9
#define TT_COLOR_GRAY 10
#define TT_COLOR_WHITE 11
#define TT_COLOR_CYAN 12
#define TT_COLOR_RED 13

#define TT_COLOR_PAIR_TITLE 1
#define TT_COLOR_PAIR_SECONDARY 2
#define TT_COLOR_PAIR_TEXT_TYPED 3
#define TT_COLOR_PAIR_TEXT_WRONG 4
#define TT_COLOR_PAIR_TEXT_TYPING 5
#define TT_COLOR_PAIR_LABEL 6
#define TT_COLOR_PAIR_LABEL_VALUE 7

#define TT_STATE_IDLE 0
#define TT_STATE_PLAY 1
#define TT_STATE_END 2

#endif
