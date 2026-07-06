#ifndef BUILTIN_H
#define BUILTIN_H

#include "parse/parser.h"

typedef struct BuiltIn {
	const char *name;
	int (*func)(ASTNode *node);
} BuiltIn;

BuiltIn find_builtin(char *command);

int cd_builtin(ASTNode *node);
int exit_builtin(ASTNode *node);
int pwd_builtin(ASTNode *node);
int echo_builtin(ASTNode *node);
int export_builtin(ASTNode *node);
int unset_builtin(ASTNode *node);
int history_builtin(ASTNode *node);

#endif