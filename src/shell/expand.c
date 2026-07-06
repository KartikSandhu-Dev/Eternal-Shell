#include "shell/expand.h"
#include "var/config.h"
#include "var/common.h"

int expand_variables(ASTNode *node) {
	size_t pos = 0;
	while(node->Command.argv[pos] != NULL) {
		if(node->Command.argv[pos][0] == SHELL_VAR_SYMBOL) {
			char *var = getenv(node->Command.argv[pos] + 1);

			if(var) {
				node->Command.argv[pos] = strdup(var);
			} else {
				fprintf(stderr, "Error: No such variable %s\n", node->Command.argv[pos]+1);
				return 1;
			}
		}

		pos++;
	}
	return 0;
}