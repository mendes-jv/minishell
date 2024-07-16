/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:07:13 by pmelo-ca          #+#    #+#             */
/*   Updated: 2024/07/16 14:07:14 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void set_env_paths(char *old_path, char *new_path, char ***envp);
static unsigned long     get_biggest_len(char *old_path, char *new_path);
static char	*get_home_dir(char **envp);

int	exec_cd(char **command, char ***envp)
{
    char *old_path;
    char *new_path;
    int exit_status;

    exit_status = 0;
    if (get_array_len(command) == 2)
    {
        old_path = getcwd(NULL, 0);
        chdir(command[1]);
        new_path = getcwd(NULL, 0);
        if (!ft_strncmp(old_path, new_path, get_biggest_len(old_path, new_path)))
        {
            dprintf(2, ERROR_CD_INVALID_PATH, command[1]);
            exit_status = 1;
        }
        else
            set_env_paths(old_path, new_path, env); //TODO validar o env
        free(old_path);
        free(new_path);
    }
    else if (get_array_len(command) > 2)
    {
        dprintf(2, ERROR_CD_MANY_ARGS);
        exit_status = 1;
    }
    else
    {
        old_path = getcwd(NULL, 0);
        new_path = get_home_dir(*envp);
        chdir(new_path);
        set_env_paths(old_path, new_path, envp);
        free(old_path);
    }
    return(exit_status);
}

static void set_env_paths(char *old_path, char *new_path, char ***envp)
{
    int arr_len;
    int i;
    char **temp;
    char **new_envp;

    i = 0;
    temp = *envp;
	arr_len = get_array_len(temp);
    new_envp = calloc(sizeof(char *), arr_len + 1);
    while(arr_len > i)
    {
        if(!ft_strncmp(temp[i] ,"PWD", 3))
            new_envp[i] = ft_strjoin("PWD=", new_path);
        else if(!ft_strncmp(temp[i], "OLDPWD", 6))
            new_envp[i] = ft_strjoin("OLDPWD=", old_path);
        else
            new_envp[i] = ft_strdup(temp[i]);
        i++;
    }
    new_envp[i] = NULL;
    *envp = new_envp;
    i = 0;
    while(temp[i])
    {
        free(temp[i]);
        i++;
    }
    free(temp);
}

static unsigned long     get_biggest_len(char *old_path, char *new_path)
{
    if (ft_strlen(new_path) >= ft_strlen(old_path))
        return(ft_strlen(new_path));
    else
        return(ft_strlen(old_path));
}

static char	*get_home_dir(char **envp)
{
    char	*home;

    home = NULL;
	while (*envp)
	{
		if (!ft_strncmp(*envp, "HOME=", 5))
		{
			home = *envp + 5;
			break;
		}
		envp++;
	}
    return (home);
}
