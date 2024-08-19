/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:07:11 by pmelo-ca          #+#    #+#             */
/*   Updated: 2024/07/18 14:06:19 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	is_builtin(char *command)
{
	if (!command)
		return (false);
	if (!ft_strncmp(command, "echo", 5) || !ft_strncmp(command, "cd", 3)
		|| !ft_strncmp(command, "pwd", 4) || !ft_strncmp(command, "export", 7)
		|| !ft_strncmp(command, "unset", 6) || !ft_strncmp(command, "env", 4)
		|| !ft_strncmp(command, "exit", 5))
		return (true);
	else
		return (false);
}

int	builtin_exec(t_minishell **minishell) //TODO: check why reddir is not working with bultins
{
	if (!ft_strncmp((*minishell)->ast->expanded_cmd[0], "echo", 5))
		return (exec_echo((*minishell)->ast->expanded_cmd));
	else if (!ft_strncmp((*minishell)->ast->expanded_cmd[0], "cd", 3))
		return (exec_cd((*minishell)->ast->expanded_cmd, &(*minishell)->env_copy));
	else if (!ft_strncmp((*minishell)->ast->expanded_cmd[0], "pwd", 4))
		return (exec_pwd((*minishell)->ast->expanded_cmd));
	else if (!ft_strncmp((*minishell)->ast->expanded_cmd[0], "export", 7))
		return (exec_export((*minishell)->ast->expanded_cmd, &(*minishell)->env_copy));
	else if (!ft_strncmp((*minishell)->ast->expanded_cmd[0], "unset", 6))
		return (exec_unset((*minishell)->ast->expanded_cmd, &(*minishell)->env_copy));
	else if (!ft_strncmp((*minishell)->ast->expanded_cmd[0], "env", 4))
		return (exec_env((*minishell)->ast->expanded_cmd, (*minishell)->env_copy));
	else if (!ft_strncmp((*minishell)->ast->expanded_cmd[0], "exit", 5))
		exec_exit((*minishell)->ast->expanded_cmd, minishell);
	return (-1);
}
