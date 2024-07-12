#include "../../includes/minishell.h"

int	exec_pwd(char **command)
{
	char	*cwd;

	if (get_array_len(command) > 1)
	{
		if (command[1][0] == '-' && command[1][1]) //TODO test second condition command[1][1]
			return (ft_printf(ERROR_PWD_ARGS, command[1]) ,2);
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (1);
	ft_printf("%s\n", cwd);
	free(cwd);
	return (0);
}
