#include "shell.h"

#include <linux/limits.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define SHELL_BUFFER_LEN 256
#define SHELL_NAME "eternal"

void shell_init() {
	char shell_buffer[SHELL_BUFFER_LEN];

	while(1) {
		print_prompt();

		read_line(shell_buffer, sizeof(shell_buffer));

		TokenList token_list = tokenize(shell_buffer);

		if (shell_buffer[0] == '\0') { continue; }

		print_tokens(&token_list);
		clean_tokens(&token_list);
	}
}

void print_prompt() {
	char cwd[PATH_MAX];
	getcwd(cwd, sizeof(cwd));

	const char *home = getenv("HOME");

	printf("%s@%s$", getlogin(), SHELL_NAME);

	if(strncmp(cwd, home, strlen(home)) == 0) {
		printf("~%s# ", cwd + strlen(home));
	} else {
		printf("~%s# ", cwd);
	}	
}

char *read_line(char *buffer, int buffer_len) {
	char *input = fgets(buffer, buffer_len, stdin);

	if(input == NULL) { printf("\n"); exit(0); }
	if(strcmp(buffer, "\n") == 0) { putchar('\n'); }

	buffer[strcspn(buffer, "\n")] = '\0';

	return buffer;
}