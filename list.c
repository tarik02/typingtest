#include "list.h"
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct list_node_t {
	struct list_node_t *prev, *next;
	void *value;
} list_node_t;

typedef struct list_t {
	list_node_t *head, *tail;
	int size;

	list_node_t *current;
	int index;
} list_t;

list_node_t *list_get_node(list_t *list, int index) {
	assert(0 <= index);
	assert(index < list->size);

	int m = list->size / 2;
	list_node_t *node;
	if (index <= m) {
		node = list->head;
		while (index--) {
			node = node->next;
		}
	} else {
		node = list->tail;
		index = list->size - index - 1;
		while (index--) {
			node = node->prev;
		}
	}

	return node;
}

list_t *list_create() {
	list_t *list = malloc(sizeof(list_t));
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;

	list->current = NULL;
	list->index = -1;

	return list;
}

void list_destroy(list_t *list) {
	list_clear(list);

	free(list);
}

int list_count(list_t *list) {
	return list->size;
}

void list_clear(list_t *list) {
	list_node_t *n = list->head;
	while (n) {
		list_node_t *on = n;
		n = n->next;
		free(on);
	}

	list->head = NULL;
	list->tail = NULL;
	list->size = 0;

	list->index = -1;
	list->current = NULL;
}

void list_add(list_t *list, void *elem) {
	list_node_t *node = malloc(sizeof(list_node_t));
	node->value = elem;

	if (list->tail == NULL) {
		node->prev = NULL;
		node->next = NULL;

		list->head = node;
		list->tail = node;
	} else {
		assert(list->tail->next == NULL);

		node->prev = list->tail;
		node->next = NULL;

		list->tail->next = node;
		list->tail = node;
	}

	++list->size;
}

void *list_remove(list_t *list, int index) {
	list_node_t *node = list_get_node(list, index);
	
	if (node->prev)
		node->prev->next = node->next;
	else
		list->head = node->next;

	if (node->next)
		node->next->prev = node->prev;
	else
		list->tail = node->prev;

	--list->size;

	void *v = node->value;
	free(node);
	return v;
}

void list_it_begin(list_t *list, int index) {
	assert(list->current == NULL);

	list->current = index == list->size ? NULL : list_get_node(list, index);
	list->index = index;
}

void list_it_end(list_t *list) {
	list->current = NULL;
	list->index = -1;
}

int list_it_prev_index(list_t *list) {
	assert(list_it_has_prev(list));

	return list->index - 1;
}

int list_it_has_prev(list_t *list) {
	assert(list->index != -1);

	return list->index != 0;
}

void *list_it_prev(list_t *list) {
	assert(list_it_has_prev(list));

	if (list->current == NULL) {
		list->current = list->tail;
	} else {
		list->current = list->current->prev;
	}
	--list->index;
	return list->current->value;
}

int list_it_next_index(list_t *list) {
	assert(list_it_has_next(list));

	return list->index;
}

int list_it_has_next(list_t *list) {
	assert(list->index != -1);

	return list->index != list->size;
}

void *list_it_next(list_t *list) {
	assert(list_it_has_next(list));

	if (list->current == NULL) {
		list->current = list_get_node(list, list->index);
	}
	void *value = list->current->value;
	list->current = list->current->next;
	++list->index;
	return value;
}
