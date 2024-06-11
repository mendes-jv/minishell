#include "../../includes/minishell.h"

static char **ft_unset_env(char *command, char **envp);

void	exec_unset(char **command, char ***envp)
{
    int	count;
    int	i;
    char **temp;
    char **new_envp;

    count = 0;
    i = 1;
    temp = *envp;
    while(temp[count])
        count++;
    while (command[i])
    {
        if (!ft_check_key(command[i]))
            printf("zapshell: unset: '%s': not a valid identifier\n", command[i]);
        else
        {
            count = 0;
            while (temp[count])
            {
                if(!ft_strncmp(command[i], temp[count], ft_strlen(command[i] + 1)))
                {
                    new_envp = ft_unset_env(command[i], temp);
                    break;
                }
                count++;
            }
        }
        i++;
    }
    *envp = new_envp;
}

static char **ft_unset_env(char *command, char **envp)
{
    char **new_envp;
    int count;

    count = 0;
    while(envp[count])
        count++;
    new_envp = malloc(sizeof(char *) * (count + 1));
    if(!new_envp)
        return (NULL);
    count = 0;
    while(envp[count])
    {
        if(ft_strncmp(command, envp[count], ft_strlen(command + 1)))
            new_envp[count] = strdup(envp[count]);
        count++;
    }
    count = 0;
    while (envp[count])
    {
        free(envp[count]);
        count++;
    }
    free(envp);
    return (new_envp);
}

int	ft_check_key(char *string)
{
    int	i;

    i = 1;
    if (!ft_isalpha(*string) && *string != '_')
        return (0);
    while (string[i] && string[i] != '=')
    {
        if (!ft_isalnum(string[i]) && string[i] != '_')
            return (0);
        i++;
    }
    return (1);
}
