#include "../../includes/minishell.h"

static void ft_set_env_paths(char *old_path, char *new_path, char ***envp);
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
        if (!strncmp(old_path, new_path, get_biggest_len(old_path, new_path)))
        {
            printf("zapshell: cd: %s: Invalid file or directory\n", command[1]);
            exit_status = 1;
        }
        else
            ft_set_env_paths(old_path, new_path, envp);
        free(old_path);
        free(new_path);
    }
    else if (get_array_len(command) > 2)
    {
        printf("zapshell: cd: too many arguments\n");
        exit_status = 1;
    }
    else
    {
        old_path = getcwd(NULL, 0);
        new_path = get_home_dir(*envp);
        chdir(new_path);
        ft_set_env_paths(old_path, new_path, envp);
        free(old_path);
    }
    return(exit_status);
}

static void ft_set_env_paths(char *old_path, char *new_path, char ***envp)
{
    int count;
    int i;
    char **temp;
    char **new_envp;

    count = 0;
    i = 0;
    temp = *envp;
    while(temp[count])
        count++;
    new_envp = malloc(sizeof(char *) * (count + 1));
    while(count > i)
    {
        if(!strncmp(temp[i] ,"PWD", 3))
            new_envp[i] = ft_strjoin("PWD=", new_path);
        else if(!strncmp(temp[i], "OLDPWD", 6))
            new_envp[i] = ft_strjoin("OLDPWD=", old_path);
        else
            new_envp[i] = strdup(temp[i]);
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
    if (strlen(new_path) >= strlen(old_path))
        return(strlen(new_path));
    else
        return(strlen(old_path));
}

static char	*get_home_dir(char **envp)
{
    char	*home;

    while (envp && ft_strncmp (*envp, "HOME=", 5))
        envp++;
    home = *envp + 5;
    return (home);
}
