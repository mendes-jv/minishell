#include "../../includes/minishell.h"

int	exec_pwd(char **command)
{
	char	*cwd;

	if (get_array_len(command) > 1)
	{
		if (command[1][0] == '-')
			return (dprintf(2, ERROR_PWD_ARGS, command[1]) ,2);
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return(1);
	dprintf(1, "%s\n", cwd);
	free(cwd);
	return(0);
}
