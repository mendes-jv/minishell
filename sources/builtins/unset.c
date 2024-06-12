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
	new_envp = NULL;
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
				if(!strncmp(command[i], temp[count], ft_strlen_env(temp[count])))
				{
					new_envp = ft_unset_env(command[i], temp);
					temp = new_envp;
					break;
				}
				count++;
			}
		}
		i++;
	}
	if (new_envp)
		*envp = new_envp;
}

static char **ft_unset_env(char *command, char **envp)
{
	char **new_envp;
	int count;
	int new_count;

	count = 0;
	new_count = 0;
	while(envp[count])
		count++;
	new_envp = malloc(sizeof(char *) * (count));
	if(!new_envp)
		return (NULL);
	count = 0;
	while(envp[count])
	{
		if(strncmp(command, envp[count], ft_strlen_env(envp[count])))
		{
			new_envp[new_count] = strdup(envp[count]);
			new_count++;
		}
		count++;
	}
	new_envp[new_count] = NULL;
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

static int ft_strlen_env(char *command)
{
	int i;

	i = 0;
	while (command[i] && command[i] != '=')
		i++;
	return (i);
}