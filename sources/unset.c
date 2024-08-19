/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:07:29 by pmelo-ca          #+#    #+#             */
/*   Updated: 2024/08/19 17:19:57 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	check_key_unset(char *string);
static char	**unset_env(char *command, char **env);
static char	**new_env_unset(char *command, char **old_env);

int	exec_unset(char **command, char ***env)
{
	int		i;
	char	**new_env;
	int		exit_status;

	i = 1;
	new_env = NULL;
	exit_status = 0;
	while (command[i])
	{
		if (!check_key_unset(command[i]))
		{
			exit_status = 1;
			dprintf(2, ERROR_UNSET_ID, command[i]);
		}
		else
			new_env = new_env_unset(command[i], *env);
		i++;
	}
	if (new_env)
		*env = new_env;
	return (exit_status);
}

static char	**new_env_unset(char *command, char **old_env)
{
	int		count;
	char	**new_env;

	count = 0;
	while (old_env[count])
	{
		if (!ft_strncmp(command, old_env[count], strlen_env(old_env[count])))
		{
			new_env = unset_env(command, old_env);
			return (new_env);
		}
		count++;
	}
	return (old_env);
}

static char	**unset_env(char *command, char **env)
{
	char	**new_env;
	int		count;
	int		new_count;

	new_count = 0;
	count = get_array_len(env);
	new_env = ft_calloc(sizeof(char *), count);
	if (!new_env)
		return (NULL);
	count = 0;
	while (env[count])
	{
		if (ft_strncmp(command, env[count], strlen_env(env[count])))
		{
			new_env[new_count] = ft_strdup(env[count]);
			new_count++;
		}
		count++;
	}
	new_env[new_count] = NULL;
	clear_matrix(env);
	return (new_env);
}

static int	check_key_unset(char *string)
{
	int	i;

	i = 1;
	if (!ft_isalpha(*string) && *string != '_')
		return (0);
	while (string[i] && string[i] != '=')
	{
		if (!ft_isalnum(string[i]) && string[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
