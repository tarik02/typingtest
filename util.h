#ifndef TT_UTIL_H
#define TT_UTIL_H

#include <ncursesw/ncurses.h>
#include <stdarg.h>
#include <stddef.h>

#define TT_STRLEN(str) (sizeof(str) / sizeof((str)[0]) - 1)

size_t utf8_len(const char *s);

int int_length(int i);

int min(int a, int b);
int max(int a, int b);

int tt_char_is_lower(char);
int tt_char_is_upper(char);

char tt_char_lower(char);
char tt_char_upper(char);

void print_str_middle(int l, int r, const char *format, ...);
void wprint_str_middle(WINDOW *win, int l, int r, const char *format, ...);
void vwprint_str_middle(WINDOW *win, int l, int r, const char *format, va_list arglist);

float errors_percent(int count, int errors);

#endif
