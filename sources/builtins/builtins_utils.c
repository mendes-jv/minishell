#include "../../includes/minishell.h"

int     get_array_len(char **arr)
{
	int arr_size;

	arr_size = 0;
	while (arr[arr_size])
		arr_size++;
	return (arr_size);
}

char	**get_env_cpy(char **envp)
{
	int count;
	int i;
	char **temp;

	count = 0;
	i = 0;
	temp = NULL;
	while(envp[count])
		count++;
	temp = malloc(sizeof(char *) * count + 1);
	while(count > i)
	{
		temp[i] = ft_strdup(envp[i]);
		i++;
	}
	return(temp);
}

int strlen_env(char *command)
{
	int i;

	i = 0;
	while (command[i] && command[i] != '=')
		i++;
	return (i);
}
