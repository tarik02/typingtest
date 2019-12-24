#ifndef TT_LIST_H
#define TT_LIST_H

typedef struct list_t list_t;

list_t *list_create();
void list_destroy(list_t *);

int list_count(list_t *);
void list_clear(list_t *);

void list_add(list_t *, void *);
void *list_remove(list_t *, int);

void list_it_begin(list_t *, int);
void list_it_end(list_t *);

int list_it_prev_index(list_t *);
int list_it_has_prev(list_t *);
void *list_it_prev(list_t *);

int list_it_next_index(list_t *);
int list_it_has_next(list_t *);
void *list_it_next(list_t *);

#endif
