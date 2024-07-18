/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:07:06 by pmelo-ca          #+#    #+#             */
/*   Updated: 2024/07/18 14:10:31 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*validate_access(char *command);
static void	clean_child_data(char **matrix, char *possible_path,
				char *part_path);

char	*get_path(char *command, char **env)
{
	char	*part_path;
	char	*possible_path;
	char	**paths;
	int		i;

	i = 0;
	if (ft_strchr(command, '/'))
		return (validate_access(command));
	while (ft_strncmp(env[i], "PATH=", 4))
		i++;
	paths = ft_split(env[i] + 5, ':');
	i = 0;
	while (paths[i])
	{
		part_path = ft_strjoin(paths[i++], "/");
		possible_path = ft_strjoin(part_path, command);
		if (!(access(possible_path, X_OK)))
		{
			clean_child_data(paths, NULL, part_path);
			return (possible_path);
		}
		clean_child_data(NULL, possible_path, part_path);
	}
	clear_matrix(paths);
	return (NULL);
}

static char	*validate_access(char *command)
{
	if (!(access(command, X_OK)))
		return (command);
	else
		return ("invalid");
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
