#include "../../includes/minishell.h"

int	exec_env(char **command, char **envp)
{
	int	count;

	count = 0;
	if (get_array_len(command) > 1)
	{
		if (command[1][0] == '-')
			return (ft_printf(ERROR_ENV_OPTION, command[1][1]), 125);
		else
			return (ft_printf(ERROR_ENV_MANY_ARGS, command[1]), 127);
	}
	while (envp[count])
	{
		ft_printf("%s\n", envp[count]);
		count++;
	}
	return 0;
}
