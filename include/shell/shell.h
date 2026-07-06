#ifndef SHELL_H
#define SHELL_H

#include "parse/lexer.h"
#include "parse/parser.h"
#include "var/config.h"

typedef struct Shell {
	char buffer[SHELL_BUFFER_LEN];

	TokenList token_list;
	ASTNode *ast;
} Shell;

void shell_init(char **envp);
void print_prompt();
char *read_line(char *buffer, const int buffer_len);

#endif