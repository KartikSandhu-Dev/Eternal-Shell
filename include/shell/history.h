#ifndef HISTROY_H
#define HISTROY_H

#include <stdlib.h>

typedef struct History {
	char **entries;
	size_t count;
	size_t capacity;
} History;

void history_init(History *history);

void history_add(History *history, char *command);
char *history_get(History *history, size_t index);

void history_print(History *history);
void history_clear(History *history);

#endif