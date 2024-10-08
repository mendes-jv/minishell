/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:07:18 by pmelo-ca          #+#    #+#             */
/*   Updated: 2024/09/02 16:52:58 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	exec_env(char **command, char **env)
{
	int		count;
	size_t	aux;

	count = 0;
	if (get_array_len(command) > 1)
	{
		if (command[1][0] == '-')
			return (dprintf(2, ERROR_ENV_OPTION, command[1][1]), 125);
		else
			return (dprintf(2, ERROR_ENV_MANY_ARGS, command[1]), 127);
	}
	while (env[count])
	{
		aux = strlen_env(env[count]);
		if (aux != ft_strlen(env[count]))
			dprintf(1, "%s\n", env[count]);
		count++;
	}
	return (0);
}
