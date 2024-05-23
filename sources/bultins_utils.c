/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bultins_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 09:11:47 by pmelo-ca          #+#    #+#             */
/*   Updated: 2024/05/23 16:43:43 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	get_biggest_len(char *old_path, char *new_path)
{
	int biggest_len;

	if (strlen(new_path) >= strlen(old_path))
		return(strlen(new_path));
	else
		return(strlen(old_path));
}

char	*get_home_dir(char **envp)
{
	char	*home;

	while (envp && ft_strncmp (*envp, "HOME=", 5))
		envp++;
	home = *envp + 5;
	return (home);
}

char	**get_env(char **envp)
{
	int count;
	int i;
	char **temp;

	count = 0;
	i = 0;
	while(envp[count])
		count++;
	temp = malloc(sizeof(char *) * count);
	while(count > i)
	{
		temp[i] = ft_strdup(envp[i]);
		i++;
	}
	return(temp);
}
