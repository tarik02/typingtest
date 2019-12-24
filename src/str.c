#include "str.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct str_t {
	int size, capacity;
	char *data;
} str_t;

str_t *str_create() {
	str_t *str = malloc(sizeof(str_t));
	str->size = 0;
	str->capacity = 32;
	str->data = malloc(str->capacity);

	return str;
}

void str_destroy(str_t *str) {
	free(str->data);
	free(str);
}

int str_length(str_t *str) {
	return str->size;
}

int str_empty(str_t *str) {
	return str->size == 0;
}

const char *str_data(str_t *str) {
	return str->data;
}

void str_clear(str_t *str) {
	str->size = 0;
}

void str_add(str_t *str, const char *other) {
	str_add_i(str, other, strlen(other));
}

void str_add_i(str_t *str, const char *other, int size) {
	if (size > str->capacity - str->size) {
		do {
			str->capacity *= 2;
		} while (size > str->capacity - str->size);

		str->data = realloc(str->data, str->capacity);
	}

	strncpy(str->data + str->size, other, size);
	str->size += size;
}

void str_add_c(str_t *str, char c) {
	str_add_i(str, &c, 1);
}

char str_pop(str_t *str) {
	assert(str->size != 0);

	return str->data[--str->size];
}
