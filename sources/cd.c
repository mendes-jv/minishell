/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:07:13 by pmelo-ca          #+#    #+#             */
/*   Updated: 2024/09/11 18:29:46 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char				*get_home_dir(char **env);
static void				cd_home_dir(char ***env);

int	exec_cd(char **command, char ***env)
{
	int		exit_status;

	exit_status = 0;
	if (get_array_len(command) == 2)
		exit_status = cd_helper(command, env);
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
