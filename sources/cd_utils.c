/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 18:18:40 by pmelo-ca          #+#    #+#             */
/*   Updated: 2024/09/11 18:29:25 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static unsigned long	get_biggest_len(char *old_path, char *new_path);

int	cd_helper(char **command, char ***env)
{
	char	*old_path;
	char	*new_path;
	int		status;

	status = 0;
	old_path = getcwd(NULL, 0);
	if (!ft_strncmp(old_path, command[1], ft_strlen(command[1])))
	{
		free(old_path);
		return (status);
	}
	chdir(command[1]);
	new_path = getcwd(NULL, 0);
	if (!ft_strncmp(old_path, new_path, get_biggest_len(old_path, new_path)))
		status = error_handler(1, 2, ERROR_EXEC_INVALID_PATH, command[1]);
	else
		set_env_paths(old_path, new_path, env);
	free(old_path);
	free(new_path);
	return (status);
}

static unsigned long	get_biggest_len(char *old_path, char *new_path)
{
	if (ft_strlen(new_path) >= ft_strlen(old_path))
		return (ft_strlen(new_path));
	else
		return (ft_strlen(old_path));
}

void	set_env_paths(char *old_path, char *new_path, char ***env)
{
	int		arr_len;
	int		i;
	char	**temp;
	char	**new_env;

	i = 0;
	temp = *env;
	arr_len = get_array_len(temp);
	new_env = calloc(sizeof(char *), arr_len + 1);
	while (arr_len > i)
	{
		if (!ft_strncmp(temp[i], "PWD", 3))
			new_env[i] = ft_strjoin("PWD=", new_path);
		else if (!ft_strncmp(temp[i], "OLDPWD", 6))
			new_env[i] = ft_strjoin("OLDPWD=", old_path);
		else
			new_env[i] = ft_strdup(temp[i]);
		i++;
	}
	new_env[i] = NULL;
	*env = new_env;
	clear_matrix(temp);
}
