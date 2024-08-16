/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:07:06 by pmelo-ca          #+#    #+#             */
/*   Updated: 2024/07/19 16:42:39 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*validate_access(char *command);

void	get_path(t_minishell **minishell)
{
	char	*part_path;
	char	*possible_path;
	char	**paths;
	int		i;

	i = 0;
	if (ft_strchr((*minishell)->ast->expanded_cmd[0], '/'))
		(*minishell)->path = validate_access((*minishell)->ast->expanded_cmd[0]);
	while (ft_strncmp((*minishell)->env_copy[i], "PATH=", 4))
		i++;
	paths = ft_split((*minishell)->env_copy[i] + 5, ':');
	i = 0;
	while (paths[i])
	{
		part_path = ft_strjoin(paths[i++], "/");
		possible_path = ft_strjoin(part_path, (*minishell)->ast->expanded_cmd[0]);
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

static char	*validate_access(char *command)
{
	char	*invalid;

	if (!(access(command, X_OK)))
		return (command);
	else
	{
		invalid = ft_strdup("invalid");
		return (invalid);
	}
}

void	reset_redirects(bool piped)
{
	if (piped)
		return ;
	dup2(STDIN_FILENO, 0);
	dup2(STDOUT_FILENO, 1);
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
