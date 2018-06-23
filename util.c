#include "util.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

size_t utf8_len(const char *s) {
	size_t count = 0;
	while (*s) {
		count += (*s++ & 0xC0) != 0x80;
	}
	return count;
}

int int_length(int i) {
	if (i == 0)
		return 1;
	if (i < 0) {
		i = -i;
	}
	return (int)log10(i) + 1;
}

int min(int a, int b) {
	return a < b ? a : b;
}

int max(int a, int b) {
	return a > b ? a : b;
}

int tt_char_is_lower(char c) {
	return 'a' <= c && c <= 'z';
}

int tt_char_is_upper(char c) {
	return 'A' <= c && c <= 'Z';
}

char tt_char_lower(char c) {
	if (tt_char_is_lower(c)) {
		return c;
	}
	if (tt_char_is_upper(c)) {
		return (c - 'A') + 'a';
	}
	return 0;
}

char tt_char_upper(char c) {
	if (tt_char_is_upper(c)) {
		return c;
	}
	if (tt_char_is_lower(c)) {
		return (c - 'a') + 'A';
	}
	return 0;
}

void print_str_middle(int l, int r, const char *format, ...) {
	va_list arglist;
	va_start(arglist, format);
	vwprint_str_middle(stdscr, l, r, format, arglist);
	va_end(arglist);
}

void wprint_str_middle(WINDOW *win, int l, int r, const char *format, ...) {
	va_list arglist;
	va_start(arglist, format);
	vwprint_str_middle(win, l, r, format, arglist);
	va_end(arglist);
}

void vwprint_str_middle(WINDOW *win, int l, int r, const char *format, va_list arglist) {
	int len = vsnprintf(NULL, 0, format, arglist);
	char *buf = malloc(len + 1);
	vsnprintf(buf, len + 1, format, arglist);

	move(getcury(stdscr), (l + r - len) / 2);
	wprintw(win, "%s", buf);
}

float errors_percent(int count, int errors) {
	if (count <= errors) return 100;

	float fcount = count;
	float ferrors = errors;

	return ((fcount - ferrors) / fcount) * 100.f;
}
