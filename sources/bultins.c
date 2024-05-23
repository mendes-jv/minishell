/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bultins.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 12:56:06 by pmelo-ca          #+#    #+#             */
/*   Updated: 2024/05/23 09:16:48 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	bultin_exec(char *command, char *args, char **envp)
{
	if (!ft_strncmp(command, "echo", 5))
		exec_echo(command, args);
	else if (!ft_strncmp(command, "cd", 3))
		exec_cd(args, envp);
	else if (!ft_strncmp(command, "pwd", 4))
		exec_pwd(args);
	else if (!ft_strncmp(command, "export", 7))
		exec_export(command, args);
	else if (!ft_strncmp(command, "unset", 6))
		exec_unset(command, args);
	else if (!ft_strncmp(command, "env", 4))
		exec_env(command, args);
	else(!ft_strncmp(command, "exit", 5))
		exec_exit(command, args);
}

int	exec_cd(char *command, char *args, char **envp)
{
	char *old_path;
	char *new_path;
	int exit_status;

	exit_status = 0;
	if (args != NULL)
	{
		old_path = getcwd(NULL, 0);
		chdir(args);
		new_path = getcwd(NULL, 0);
		if (!ft_strncmp(old_path, new_path, get_biggest_len(old_path, new_path)))
		{
			ft_printf("zapshell: cd: %s: Invalid file or directory\n", args);
			exit_status = 1;
		}
		free(old_path);
		free(new_path);
	}
	else
	{
		new_path = get_home_dir(envp);
		chdir(new_path);
	}
	return(exit_status);
}

void	exec_pwd(char *args)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	ft_printf("%s\n", pwd);
	free(pwd);
}

