/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:07:08 by pmelo-ca          #+#    #+#             */
/*   Updated: 2024/08/19 17:16:59 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	get_array_len(char **arr)
{
	int	arr_size;

	arr_size = 0;
	if (!arr)
		return (0);
	while (arr[arr_size])
		arr_size++;
	return (arr_size);
}

char	**get_env_cpy(char **envp)
{
	int		count;
	int		i;
	char	**temp;

	count = 0;
	i = 0;
	temp = NULL;
	while (envp[count])
		count++;
	temp = ft_calloc(sizeof(char *), count + 1);
	while (count > i)
	{
		temp[i] = ft_strdup(envp[i]);
		i++;
	}
	return (temp);
}

int	strlen_env(char *command)
{
	int	i;

	i = 0;
	while (command[i] && command[i] != '=')
		i++;
	return (i);
}

void	list_remove_if(t_list **begin_list, void *data_ref)
{
	t_list	*cur;

	if (begin_list == NULL || *begin_list == NULL)
		return ;
	cur = *begin_list;
	if (!ft_strncmp(cur->content, data_ref, ft_strlen(data_ref)))
	{
		*begin_list = cur->next;
		free(cur);
	}
	else
	{
		cur = *begin_list;
		list_remove_if(&cur->next, data_ref);
	}
}

int	error_handler(int exit_status, int fd, char *message, char *command)
{
	dprintf(fd, message, command);
	return (exit_status);
}
