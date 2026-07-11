#include "shell/shell.h"
/* bug to fix
dome@devuan:~/LshShell$ lsh
[lsh]@~/LshShell# cat < file > out
open: No such file or directory       
free(): double free detected in tcache 2                                    
Aborted  
*/

int main(int argc, char *argv[], char *envp[])
{
	(void)argc;
	(void)argv;
	
	// run shell
	shell_init(envp);
	return 0;
}
