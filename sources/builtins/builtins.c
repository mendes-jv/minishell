#include "../../includes/minishell.h"

void	bultin_exec(char **command, char **envp)
{
    char	**envp_cpy;

    envp_cpy = ft_get_env_cpy(envp);
    if (!ft_strncmp(command[0], "echo", 5))
        exec_echo(command);
    else if (!ft_strncmp(command[0], "cd", 3))
        exec_cd(command, &envp_cpy);
    else if (!ft_strncmp(command[0], "pwd", 4))
        exec_pwd(command);
    else if (!ft_strncmp(command[0], "export", 7))
        exec_export(command, envp_cpy);
    else if (!ft_strncmp(command[0], "unset", 6))
        exec_unset(command, &envp_cpy);
    else if (!ft_strncmp(command[0], "env", 4))
        exec_env(command, envp_cpy);
    else if (!ft_strncmp(command[0], "exit", 5))
        exec_exit(command);
}
