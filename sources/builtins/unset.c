/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:07:29 by pmelo-ca          #+#    #+#             */
/*   Updated: 2024/07/16 14:07:38 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	check_key_unset(char *string);
static char **unset_env(char *command, char **envp);

int		exec_unset(char **command, char ***envp)
{
	int	count;
	int	i;
	char **temp;
	char **new_envp;
	int 	exit_status;

	i = 1;
	temp = *envp;
	new_envp = NULL;
	exit_status = 0;
	while (command[i])
	{
		if (!check_key_unset(command[i]))
		{
			exit_status = 2;
			dprintf(2, ERROR_UNSET_ID, command[i]);
		}
		else
		{
			count = 0;
			while (temp[count])
			{
				if(!ft_strncmp(command[i], temp[count], strlen_env(temp[count])))
				{
					new_envp = unset_env(command[i], temp);
					temp = new_envp;
					break;
				}
				count++;
			}
		}
		i++;
	}
	if (new_envp)
		*envp = new_envp;
	return(exit_status);
}

static char **unset_env(char *command, char **envp)
{
	char **new_envp;
	int count;
	int new_count;

	new_count = 0;
	count = get_array_len(envp);
	new_envp = ft_calloc(sizeof(char *), count);
	if(!new_envp)
		return (NULL);
	count = 0;
	while(envp[count])
	{
		if(ft_strncmp(command, envp[count], strlen_env(envp[count])))
		{
			new_envp[new_count] = ft_strdup(envp[count]);
			new_count++;
		}
		count++;
	}
	new_envp[new_count] = NULL;
	count = 0;
	while (envp[count])
	{
		free(envp[count]);
		count++;
	}
	free(envp);
	return (new_envp);
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

