#include "typingtest.h"
#include "locale/conclusions.h"
#include "locale/messages.h"
#include "words.h"
#include "util.h"
#include "list.h"
#include "str.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <locale.h>
#include <ncurses.h>

#if defined(TT_UNIX)
#include <pthread.h>
#elif defined(TT_WINDOWS)
#include <Windows.h>
#endif

#if TT_SIGNALS
#include <signal.h>
#endif

bool should_exit = false;

#if TT_SIGNALS
void signal_quit(__attribute__((unused)) int signum) {
	signal(SIGQUIT, signal_quit);

	should_exit = true;
}

void signal_interrupt(__attribute__((unused)) int signum) {
	signal(SIGINT, signal_interrupt);

	should_exit = true;
}
#endif

int old_height = 0, old_width = 0;
int old_time = 0;

WINDOW *win_title, *win_text, *win_stats, *win_end = NULL;

int state = TT_STATE_IDLE;
int state_changed = 0;
int words = 0, chars = 0, errors = 0;

list_t *next_words, *typed_words;

str_t *back_word, *back_word_wrong;
str_t *current_word;

void add_next_word() {
	int wi = rand() % tt_words_count;
	list_add(next_words, (void *)tt_words[wi]);
}

void check_next_words(int count) {
	for (int i = list_count(next_words); i <= count; ++i) {
		add_next_word();
	}
}

void take_next_word() {
	int len = str_length(current_word);

	if (len) {
		bool has_error = !str_empty(back_word_wrong) || str_length(back_word) != str_length(current_word);

		if (!has_error) {
			++words;
			chars += len;

			str_t *str = str_create();
			str_add_c(str, '1');
			str_add_i(str, str_data(current_word), len);
			list_add(typed_words, str);
		} else {
			++errors;

			str_t *str = str_create();
			str_add_c(str, '0');
			str_add_i(str, str_data(back_word), str_length(back_word));
			str_add_i(str, str_data(back_word_wrong), str_length(back_word_wrong));
			list_add(typed_words, str);
		}
	}

	str_clear(back_word);
	str_clear(back_word_wrong);
	str_clear(current_word);

	check_next_words(1);
	str_add(current_word, (const char *)list_remove(next_words, 0));
}

void clear_typed_words() {
	list_it_begin(typed_words, 0);
	while (list_it_has_next(typed_words)) {
		str_destroy(list_it_next(typed_words));
	}
	list_it_end(typed_words);

	list_clear(typed_words);
}

void game_start() {
	state = TT_STATE_PLAY;
	state_changed = old_time;
	
	words = 0;
	chars = 0;
	errors = 0;
}

void game_stop() {
	state = TT_STATE_END;
	state_changed = old_time;

	clear_typed_words();

	str_clear(back_word);
	str_clear(back_word_wrong);
	str_clear(current_word);
	take_next_word();

	if (win_end) delwin(win_end);
	win_end = newwin(0, 0, 0, 0);
}

void tt_begin() {
	win_title = newwin(0, 0, 0, 0);
	win_text = newwin(0, 0, 0, 0);
	win_stats = newwin(0, 0, 0, 0);

	srand(tt_time());

	next_words = list_create();
	typed_words = list_create();

	back_word = str_create();
	back_word_wrong = str_create();
	current_word = str_create();

	take_next_word();
}

void tt_end() {
	str_destroy(current_word);
	str_destroy(back_word_wrong);
	str_destroy(back_word);

	clear_typed_words();

	list_destroy(typed_words);
	list_destroy(next_words);

	if (win_end) {
		delwin(win_end);
		win_end = NULL;
	}
	
	delwin(win_stats);
	delwin(win_text);
	delwin(win_title);
}

void update() {
	tt_sleep(1);
	curs_set(0);

	bool full_redraw = false;
	
	int height, width;
	getmaxyx(stdscr, height, width);

	if (height != old_height || width != old_width) {
		old_height = height;
		old_width = width;

		full_redraw = true;
	}

	int time = tt_time();
	if (time != old_time) {
		old_time = time;
		

	}

	int c = 0;
	while ((c = getch()) != ERR) {
		switch (state) {
		case TT_STATE_IDLE:
		case TT_STATE_PLAY:
			switch (c) {
			case 10: // enter
			case 32: // space
				if (!(str_empty(back_word_wrong) && str_empty(back_word))) {
					take_next_word();
				}
				break;
			case 27: // esc
				if (state == TT_STATE_PLAY) {
					game_stop();
				}
				break;
			case 263: // backspace
			case 330: // delete
				if (str_length(back_word_wrong)) {
					str_pop(back_word_wrong);
				} else if (str_length(back_word)) {
					str_pop(back_word);
				} else {
					beep();
				}
				break;
			default:
				if (c != '\'') {
					c = tt_char_lower(c);
					if (!c) break;
				}

				if (state == TT_STATE_IDLE) {
					game_start();
				}

				int wrongc = str_length(back_word_wrong);
				int rightc = str_length(back_word);
				int cc = wrongc + rightc;
				if (cc > 15) { // TODO: constant
					beep();
				} else if (wrongc != 0) {
					str_add_c(back_word_wrong, c);
					beep();
				} else {
					if (str_data(current_word)[cc] == c && cc + 1 <= str_length(current_word)) {
						str_add_c(back_word, c);
					} else {
						str_add_c(back_word_wrong, c);
						beep();
					}
				}
			}
			break;
		case TT_STATE_END:
			if ((3 + 1) - (time - state_changed) <= 0) {
				state = TT_STATE_IDLE;
				state_changed = time;

				if (win_end) {
					delwin(win_end);
					win_end = NULL;
				}
				full_redraw = true;
			}
			break;
		}
	}

	if (full_redraw) {
		clear();
		refresh();
	}

	if (full_redraw) {
		int l = utf8_len(msg_title);
		wresize(win_title, 1, l);
		mvwin(win_title, 1, (width - l) / 2);
		wclear(win_title);
		wmove(win_title, 0, 0);

		wattron(win_title, A_BOLD | COLOR_PAIR(TT_COLOR_PAIR_TITLE));
		wprintw(win_title, "%s", msg_title);
		wattroff(win_title, A_BOLD);

		wrefresh(win_title);
	}

	do {
		int msg_seconds_len = utf8_len(msg_seconds);
		int msg_words_per_minute_len = utf8_len(msg_words_per_minute);
		int msg_chars_per_minute_len = utf8_len(msg_chars_per_minute);
		int msg_percent_accuracy_len = utf8_len(msg_percent_accuracy);

		int nw = max(msg_seconds_len,
			msg_words_per_minute_len + 5 + msg_chars_per_minute_len + 5 + msg_percent_accuracy_len);
		wresize(win_stats, 5, nw);
		mvwin(win_stats, height - 6, (width - nw) / 2);

		int xs0 = (nw - msg_seconds_len) / 2;
		int xs1 = 0;
		int xs2 = xs1 + msg_words_per_minute_len + 5;
		int xs3 = xs2 + msg_chars_per_minute_len + 5;

		wmove(win_stats, 0, 0);
		wclrtoeol(win_stats);
		wmove(win_stats, 3, 0);
		wclrtoeol(win_stats);

		if (full_redraw) {
			wattron(win_stats, A_BOLD | COLOR_PAIR(TT_COLOR_PAIR_LABEL));

			mvwprintw(win_stats, 1, xs0, "%s", msg_seconds);
			mvwprintw(win_stats, 4, xs1, "%s     %s     %s", 
				msg_words_per_minute, 
				msg_chars_per_minute,
				msg_percent_accuracy);

			wattroff(win_stats, A_BOLD);
		}

		wattron(win_stats, COLOR_PAIR(TT_COLOR_PAIR_LABEL_VALUE));
		do {
			int rem;
			switch (state) {
			case TT_STATE_IDLE:
				rem = 60;
				break;
			case TT_STATE_PLAY:
				rem = (state_changed + 60) - time;
				if (rem < 0) {
					game_stop();
				}
				break;
			case TT_STATE_END:
				rem = 0;
				break;
			}

			int ilen = int_length(rem);
			mvwprintw(win_stats, 0, xs0 + (msg_seconds_len - ilen) / 2, "%d", rem);
		} while (0);

		do {
			int ilen = int_length(words);
			mvwprintw(win_stats, 3, xs1 + (msg_words_per_minute_len - ilen) / 2, "%d", words);
		} while (0);

		do {
			int ilen = int_length(chars);
			mvwprintw(win_stats, 3, xs2 + (msg_chars_per_minute_len - ilen) / 2, "%d", chars);
		} while (0);

		do {
			int accuracy = round(errors_percent(chars, errors));
			int ilen = int_length(accuracy);
			mvwprintw(win_stats, 3, xs3 + (msg_percent_accuracy_len - ilen) / 2, "%d", accuracy);
		} while (0);

		wrefresh(win_stats);
	} while (0);

	do {
		if (state != TT_STATE_END || full_redraw) {
			int mrow = 2 + (height - 8) / 2;
			int mcol = width / 2;
			
			wresize(win_text, 3, width);
			mvwin(win_text, mrow - 1, 0);

			wmove(win_text, 1, 0);
			wclrtoeol(win_text);

			if (full_redraw) {
				wmove(win_text, 0, 0);
				wclrtoeol(win_text);

				wmove(win_text, 2, 0);
				wclrtoeol(win_text);

				mvwprintw(win_text, 0, mcol, "|");
				mvwprintw(win_text, 2, mcol, "|");
			}

			wmove(win_text, 1, 0);

			int bwl = str_length(back_word);
			int bwwl = str_length(back_word_wrong);

			mcol -= bwl + bwwl;
			wmove(win_text, 1, mcol);
			int rem = width - mcol;

			do {
				int scol = mcol;

				list_it_begin(typed_words, list_count(typed_words));
				while (list_it_has_prev(typed_words)) {
					str_t *str = list_it_prev(typed_words);
					const char *data = str_data(str) + 1;
					int len = str_length(str) - 1;

					if (data[-1] == '0') {
						wattron(win_text, COLOR_PAIR(TT_COLOR_PAIR_TEXT_WRONG));
					} else {
						wattron(win_text, COLOR_PAIR(TT_COLOR_PAIR_TEXT_TYPED));
					}

					scol -= len + 1;
					if (scol < 0) {
						if (scol + len <= 0) break;

						data += -scol;
						len += scol;
						scol = 0;
					}
					mvwprintw(win_text, 1, scol, "%.*s", len, data);
				}
				list_it_end(typed_words);
			} while (0);

			wmove(win_text, 1, mcol);

			do {
				wattron(win_text, COLOR_PAIR(TT_COLOR_PAIR_TEXT_TYPED));
				wprintw(win_text, "%.*s", min(bwl, rem), str_data(back_word));
				rem -= bwl;

				wattron(win_text, COLOR_PAIR(TT_COLOR_PAIR_TEXT_WRONG));
				wprintw(win_text, "%.*s", min(bwwl, rem), str_data(back_word_wrong));
				rem -= bwwl;

				wattron(win_text, COLOR_PAIR(TT_COLOR_PAIR_TEXT_TYPING));
				int l = max(0, str_length(current_word) - bwl);
				wprintw(win_text, "%.*s", l, str_data(current_word) + bwl);

				rem -= l;
				--rem;
			} while (0);

			waddch(win_text, ' ');

			list_it_begin(next_words, 0);
			while (rem) {
				if (!list_it_has_next(next_words)) {
					add_next_word();
				}

				const char *word = (const char *)list_it_next(next_words);
				int l = strlen(word);

				waddnstr(win_text, word, min(rem, l));
				waddch(win_text, ' ');
				rem -= l;
				--rem;

				if (rem <= 0) break;
			}
			list_it_end(next_words);
		}

		wrefresh(win_text);
	} while (0);

	if (state == TT_STATE_END) {
		assert(win_end);

		int wwidth = min(width, 60);
		int wheight = 10;

		wresize(win_end, wheight, wwidth);
		mvwin(win_end, 2 + (height - (2 + 6) - wheight) / 2, (width - wwidth) / 2);

		box(win_end, 0, 0);

		int mark = words - errors;
		const conclusion_t *conclusion = NULL;
		for (int i = 0; i < tt_conclusions_count; ++i) {
			conclusion = &tt_conclusions[i];
			if (conclusion->condition == -1 || mark < conclusion->condition) {
				break;
			}
		}
		assert(conclusion);

		int padding_left = 0;
		for (int i = 0; i < TT_CONCLUSION_ART_COUNT; ++i) {
			padding_left = max(padding_left, strlen(conclusion->art[i]));
			mvwprintw(win_end, 1 + i, 2, conclusion->art[i]);
		}
		
		padding_left += 4;
		mvwprintw(win_end, 2, padding_left, conclusion->message1);
		mvwprintw(win_end, 3, padding_left, conclusion->message2);
		mvwprintw(win_end, 4, padding_left, conclusion->message3);

		int rem = (3 + 1) - (time - state_changed);
		if (rem <= 0) {
			mvwprintw(win_end, 8, 2, "%s", msg_any_key_continue);
		} else {
			wattron(win_end, COLOR_PAIR(TT_COLOR_PAIR_SECONDARY));
			mvwprintw(win_end, 8, 2, msg_wait_seconds, rem);
			wattroff(win_end, COLOR_PAIR(TT_COLOR_PAIR_SECONDARY));
		}

		wrefresh(win_end);
	}

	refresh();
}

int main() {
	setlocale(LC_ALL, "");

#if TT_SIGNALS
	signal(SIGQUIT, signal_quit);
	signal(SIGINT, signal_interrupt);
#endif

	initscr();
	keypad(stdscr, TRUE);
	cbreak();
	noecho();
	nodelay(stdscr, TRUE);

	start_color();

	if (COLORS > 8) {
		init_color(TT_COLOR_BLACK, 0, 0, 0);
		init_color(TT_COLOR_GRAY, 500, 500, 500);
		init_color(TT_COLOR_WHITE, 1000, 1000, 1000);
		init_color(TT_COLOR_CYAN, 0, 700, 700);
		init_color(TT_COLOR_RED, 1000, 0, 0);

		init_pair(TT_COLOR_PAIR_TITLE, TT_COLOR_GRAY, TT_COLOR_BLACK);
		init_pair(TT_COLOR_PAIR_SECONDARY, TT_COLOR_GRAY, TT_COLOR_BLACK);
		init_pair(TT_COLOR_PAIR_TEXT_TYPED, TT_COLOR_GRAY, TT_COLOR_BLACK);
		init_pair(TT_COLOR_PAIR_TEXT_WRONG, TT_COLOR_RED, TT_COLOR_BLACK);
		init_pair(TT_COLOR_PAIR_TEXT_TYPING, TT_COLOR_WHITE, TT_COLOR_BLACK);
		init_pair(TT_COLOR_PAIR_LABEL, TT_COLOR_CYAN, TT_COLOR_BLACK);
		init_pair(TT_COLOR_PAIR_LABEL_VALUE, TT_COLOR_WHITE, TT_COLOR_BLACK);
	} else {
		init_pair(TT_COLOR_PAIR_TITLE, COLOR_CYAN, COLOR_BLACK);
		init_pair(TT_COLOR_PAIR_SECONDARY, COLOR_MAGENTA, COLOR_BLACK);
		init_pair(TT_COLOR_PAIR_TEXT_TYPED, COLOR_CYAN, COLOR_BLACK);
		init_pair(TT_COLOR_PAIR_TEXT_WRONG, COLOR_RED, COLOR_BLACK);
		init_pair(TT_COLOR_PAIR_TEXT_TYPING, COLOR_WHITE, COLOR_BLACK);
		init_pair(TT_COLOR_PAIR_LABEL, COLOR_CYAN, COLOR_BLACK);
		init_pair(TT_COLOR_PAIR_LABEL_VALUE, COLOR_WHITE, COLOR_BLACK);
	}
	
	bkgd(COLOR_PAIR(TT_COLOR_PAIR_TITLE));

	refresh();

	tt_begin();
	while (!should_exit) {
		update();
	}
	tt_end();

	endwin();
	return 0;
}
