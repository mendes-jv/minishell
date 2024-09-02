/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:07:06 by pmelo-ca          #+#    #+#             */
/*   Updated: 2024/09/02 17:09:03 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	get_path(t_minishell **minishell)
{
	char	*part_path;
	char	*possible_path;
	char	**paths;
	int		i;

	i = 0;
	if ((ft_strchr((*minishell)->ast->expanded_cmd[0], '/'))
		|| (*minishell)->ast->expanded_cmd[0][0] == '.')
		(*minishell)->path = ft_strdup((*minishell)->ast->expanded_cmd[0]);
	while (ft_strncmp((*minishell)->env_copy[i], "PATH=", 4))
		i++;
	paths = ft_split((*minishell)->env_copy[i] + 5, ':');
	i = 0;
	while (paths[i])
	{
		part_path = ft_strjoin(paths[i++], "/");
		possible_path = ft_strjoin(part_path,
				(*minishell)->ast->expanded_cmd[0]);
		if (!(access(possible_path, X_OK)))
		{
			clean_child_data(paths, NULL, part_path);
			(*minishell)->path = possible_path;
			return ;
		}
		clean_child_data(NULL, possible_path, part_path);
	}
	clear_matrix(paths);
}

void	validate_access(t_minishell **minishell)
{
	if (!(*minishell)->path)
		exit_handler(ERROR_EXEC_COM_NOT_FOUND,
			(*minishell)->ast->expanded_cmd[0], minishell, true, 127);
	if (access((*minishell)->ast->expanded_cmd[0], F_OK) != 0)
		exit_handler(ERROR_EXEC_INVALID_PATH,
			(*minishell)->ast->expanded_cmd[0], minishell, true, 127);
	else if (access((*minishell)->ast->expanded_cmd[0], X_OK) == 0)
		exit_handler(ERROR_EXEC_DIRECTORY, (*minishell)->ast->expanded_cmd[0],
			minishell, true, 126);
	else
		exit_handler(ERROR_EXEC_PERMISSION_DENY,
			(*minishell)->ast->expanded_cmd[0], minishell, true, 126);
}

void	reset_redirects(bool piped, t_minishell *minishell)
{
	if (piped)
		return ;
	dup2(minishell->stdin, 0);
	dup2(minishell->stdout, 1);
}

bool	isvalid_num(char *command)
{
	long long	c;
	char		*string;

	if (command[0] == '+' || command[0] == '-')
		command++;
	if (isnumber(command))
	{
		c = atoll(command);
		string = lltoa(c);
		if (ft_strncmp(string, command, ft_strlen(command)) == 0)
		{
			free(string);
			return (true);
		}
		else
		{
			free(string);
			return (false);
		}
	}
	else
		return (false);
}

void	exit_handler(char *message, char *command, t_minishell **minishell,
	bool print, int exit_status)
{
	if (print == true)
		dprintf(2, message, command);
	clear_minishell(*minishell);
	exit(exit_status);
}
