#ifndef TT_STR_H
#define TT_STR_H

typedef struct str_t str_t;

str_t *str_create();
void str_destroy(str_t *);

int str_length(str_t *);
int str_empty(str_t *);

const char *str_data(str_t *);
void str_clear(str_t *);

void str_add(str_t *, const char *);
void str_add_i(str_t *, const char *, int);
void str_add_c(str_t *, char);

char str_pop(str_t *);

// void strqueue_

#endif
