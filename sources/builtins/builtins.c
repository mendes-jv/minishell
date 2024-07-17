/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:07:11 by pmelo-ca          #+#    #+#             */
/*   Updated: 2024/07/16 14:07:12 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int	builtin_exec(char **command, char **envp)
{
    char	**envp_cpy;

    envp_cpy = get_env_cpy(envp);
    if (!ft_strncmp(command[0], "echo", 5))
        return(exec_echo(command));
    else if (!ft_strncmp(command[0], "cd", 3))
		return(exec_cd(command, &envp_cpy));
    else if (!ft_strncmp(command[0], "pwd", 4))
		return(exec_pwd(command));
    else if (!ft_strncmp(command[0], "export", 7))
		return(exec_export(command, &envp_cpy));
    else if (!ft_strncmp(command[0], "unset", 6))
		return(exec_unset(command, &envp_cpy));
    else if (!ft_strncmp(command[0], "env", 4))
		return(exec_env(command, envp_cpy));
    else if (!ft_strncmp(command[0], "exit", 5))
        exec_exit(command);
	return(0); // TODO check
}
