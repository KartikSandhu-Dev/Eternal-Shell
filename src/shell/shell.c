#include "shell/shell.h"
#include "var/config.h"
#include "var/common.h"

#include "parse/lexer.h"
#include "parse/parser.h"
#include "exec/execute.h"
#include "shell/history.h"

#include <unistd.h>
#include <sys/wait.h>
#include <linux/limits.h>

void shell_init(char **envp) {
	Shell shell;
	history_init(&shell.history);

	while(1) {
		print_prompt();

		char *line = read_line(shell.buffer, sizeof(shell.buffer));
		if(line[0] == '\0' || !line) { continue; }

		history_add(&shell.history, line);

		shell.token_list = tokenize(shell.buffer);

		shell.ast = parse_tokens(&shell.token_list);

		execute(shell.ast, envp);

		clean_ASTs(shell.ast);
		clean_tokens(&shell.token_list); 
	}
}

void print_prompt() {
	char cwd[PATH_MAX];
	getcwd(cwd, sizeof(cwd));

	const char *home = getenv("HOME");

	printf("%s@", SHELL_NAME);

	if(strncmp(cwd, home, strlen(home)) == 0) {
		printf("~%s$ ", cwd + strlen(home));
	} else {
		printf("~%s$ ", cwd);
	}

	fflush(stdout);
}

char *read_line(char *buffer, const int buffer_len) {
	char *input = fgets(buffer, buffer_len, stdin);

	if (!input) { return NULL; }

	buffer[strcspn(buffer, "\n")] = '\0';

	return buffer;
}