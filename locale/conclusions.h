#ifndef TT_LOCALE_CONCLUSIONS_H
#define TT_LOCALE_CONCLUSIONS_H

#define TT_CONCLUSION_ART_COUNT 6

typedef struct conclusion_t {
	int condition;
	const char *name;
	const char *art[TT_CONCLUSION_ART_COUNT];
	const char *message1;
	const char *message2;
	const char *message3;
} conclusion_t;

extern conclusion_t tt_conclusions[];
extern const int tt_conclusions_count;

#endif
