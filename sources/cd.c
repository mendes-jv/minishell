/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:07:13 by pmelo-ca          #+#    #+#             */
/*   Updated: 2024/08/19 17:17:09 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void				set_env_paths(char *old_path, char *new_path,
							char ***env);
static unsigned long	get_biggest_len(char *old_path, char *new_path);
static char				*get_home_dir(char **env);
static void				cd_home_dir(char ***env);

int	exec_cd(char **command, char ***env)
{
	char	*old_path;
	char	*new_path;
	int		exit_status;

	exit_status = 0;
	if (get_array_len(command) == 2)
	{
		old_path = getcwd(NULL, 0);
		if (!ft_strncmp(old_path, command[1], ft_strlen(command[1])))
		{
			free(old_path);
			return (exit_status);
		}
		chdir(command[1]);
		new_path = getcwd(NULL, 0);
		if (!ft_strncmp(old_path, new_path, get_biggest_len(old_path,
					new_path)))
			exit_status = error_handler(1, 2, ERROR_EXEC_INVALID_PATH,
					command[1]);
		else
			set_env_paths(old_path, new_path, env);
		free(old_path);
		free(new_path);
	}
	else if (get_array_len(command) > 2)
		exit_status = error_handler(1, 2, ERROR_CD_MANY_ARGS, NULL);
	else
		cd_home_dir(env);
	return (exit_status);
}

static void	cd_home_dir(char ***env)
{
	char	*old_path;
	char	*new_path;

	old_path = getcwd(NULL, 0);
	new_path = get_home_dir(*env);
	chdir(new_path);
	set_env_paths(old_path, new_path, env);
	free(old_path);
}

static void	set_env_paths(char *old_path, char *new_path, char ***env)
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

static unsigned long	get_biggest_len(char *old_path, char *new_path)
{
	if (ft_strlen(new_path) >= ft_strlen(old_path))
		return (ft_strlen(new_path));
	else
		return (ft_strlen(old_path));
}

static char	*get_home_dir(char **env)
{
	char	*home;

	home = NULL;
	while (*env)
	{
		if (!ft_strncmp(*env, "HOME=", 5))
		{
			home = *env + 5;
			break ;
		}
		env++;
	}
	return (home);
}
