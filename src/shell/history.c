#include "shell/history.h"
#include "var/config.h"
#include "var/common.h"

void history_init(History *history) {
	history->count = 0;
	history->capacity = HISTORY_CAPACITY;
	history->entries = malloc(sizeof(char *)*history->capacity);
}

void history_add(History *history, char *command) {
	if(history->count >= history->capacity) {
		history->capacity += 50;
		history->entries = realloc(history->entries, sizeof(char *)*history->capacity);
	}

	history->entries[history->count] = strdup(command);
	history->count++;
}

char *history_get(History *history, size_t index) {
	if(index > history->capacity) {
		return NULL;
	}
	return history->entries[index];
}

void history_print(History *history) {
	for(size_t i = 0; i < history->count; i++) {
		printf("%zu. %s\n", i, history->entries[i]);
	}
}


void history_clean(History *history) {
	for(size_t i = 0; i < history->count; i++) {
		free(history->entries[i]);
	}

	free(history->entries);

	history->count = 0;
	history->capacity = 0;
}