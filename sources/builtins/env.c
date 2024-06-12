#include "../../includes/minishell.h"

int	exec_env(char **command, char **envp)
{
	int	count;

	count = 0;
	if (get_array_len(command) > 1)
	{
		if (command[1][0] == '-')
			return (printf("env: invalid option -- '%c'\n", command[1][1]), 125);
		else
			return (printf("env: '%s': No such file or directory\n", command[1]), 127);
	}
	while (envp[count])
	{
		printf("%s\n", envp[count]);
		count++;
	}
	return 0;
}
