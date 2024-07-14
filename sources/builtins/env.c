#include "../../includes/minishell.h"

int	exec_env(char **command, char **envp)
{
	int	count;

	count = 0;
	if (get_array_len(command) > 1)
	{
		if (command[1][0] == '-')
			return (dprintf(2, ERROR_ENV_OPTION, command[1][1]), 125);
		else
			return (dprintf(2, ERROR_ENV_MANY_ARGS, command[1]), 127);
	}
	while (envp[count])
	{
		dprintf(1, "%s\n", env[count]); //TODO  validate env
		count++;
	}
	return (0);
}
