/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bultins.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 12:56:06 by pmelo-ca          #+#    #+#             */
/*   Updated: 2024/05/24 19:49:39 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	bultin_exec(char *command, char *args, char **envp)
{
	char	**envp_cpy;

	envp_cpy = ft_get_env(envp);
	if (!ft_strncmp(command, "echo", 5))
		exec_echo(args);
	else if (!ft_strncmp(command, "cd", 3))
		exec_cd(args, envp_cpy);
	else if (!ft_strncmp(command, "pwd", 4))
		exec_pwd(args);
	else if (!ft_strncmp(command, "export", 7))
		exec_export(args, envp_cpy);
	else if (!ft_strncmp(command, "unset", 6))
		exec_unset(command, args);
	else if (!ft_strncmp(command, "env", 4))
		exec_env(args, envp_cpy);
	else if (!ft_strncmp(command, "exit", 5))
		exec_exit();
}

void	exec_echo(char *args)
{
	if (args[0] == '-'&& args[1] == 'n')
	{
		args += 2;
		ft_printf("%s", args);
	}
	else
		ft_printf("%s\n", args);
}

int	exec_cd(char *args, char **envp)
{
	//preciso ainda validar se altero o oldpwd e o pwd na env com o chdir automaticamente;
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

void	exec_export(char *args, char **envp)
{
	int	count;
	int	i;
	char	**temp;

	count = 0;
	i = 0;
	if (args)
	{
		while(envp[count])
			count++;
		temp = malloc(sizeof(char *) * (count + 1));
		while(count > i)
		{
			temp[i] = ft_strdup(envp[i]);
			i++;
		}
		temp[i] = ft_strdup(args);
		free(envp);
	}
	else if (!args)
	{
		//printar em ordem alfabeto ascii;
	}
	//precisa atualizar o ponteiro do env
}

void	exec_unset(char *args, char **envp)
{
	char *temp;
	int	count;
	int	i;

	count = 0;
	i = 0;
	while(envp[count])
		count++;
	temp = malloc(sizeof(char) * (count - 1));
	while(count > i)
	{
		if (ft_strncmp(envp[i], get_env(args), ft_strlen(envp[i])))
			temp[i] = ft_strdup(envp[i]);
		i++;
	}
	free(envp);
	//precisa atualizar o ponteiro do env
}

void	exec_env(char *args, char **envp)
{
	int	count;

	count = 0;
	if(args)
	{
		ft_printf("env: invalid option -- %s\n", args);
		return;
	}
	while (envp[count])
	{
		ft_printf("%s\n", envp[count]);
		count++;
	}
}

