#include "../../includes/minishell.h"

bool	is_builtin(char *command)
{
	if (!command)
		return (false);
	if (!ft_strncmp(command, "echo", 5)
			|| !ft_strncmp(command, "cd", 3)
			|| !ft_strncmp(command, "pwd", 4)
			|| !ft_strncmp(command, "export", 7)
			|| !ft_strncmp(command, "unset", 6)
			|| !ft_strncmp(command, "env", 4)
			|| !ft_strncmp(command, "exit", 5))
			return (true);
	else
		return (false);
}

void	bultin_exec(char **command, char **envp)
{
    char	**envp_cpy;

    envp_cpy = get_env_cpy(envp);
    if (!ft_strncmp(command[0], "echo", 5))
        exec_echo(command);
    else if (!ft_strncmp(command[0], "cd", 3))
        exec_cd(command, &envp_cpy);
    else if (!ft_strncmp(command[0], "pwd", 4))
        exec_pwd(command);
    else if (!ft_strncmp(command[0], "export", 7))
        exec_export(command, &envp_cpy);
    else if (!ft_strncmp(command[0], "unset", 6))
        exec_unset(command, &envp_cpy);
    else if (!ft_strncmp(command[0], "env", 4))
        exec_env(command, envp_cpy);
    else if (!ft_strncmp(command[0], "exit", 5))
        exec_exit(command);
}

