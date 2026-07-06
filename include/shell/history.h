#ifndef HISTROY_H
#define HISTROY_H

#include <stdlib.h>

typedef struct History {
	char **entries;
	size_t count;
	size_t capacity;
} History;

void history_init();

void history_add(char *command);
char *history_get(size_t index);

void history_print();
void history_clear();

#endif