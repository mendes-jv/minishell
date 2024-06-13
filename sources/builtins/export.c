#include "../../includes/minishell.h"

static int	ft_check_key_export(char *string);
static void ft_print_env_sorted(char **envp);

int	exec_export(char **command, char ***envp)
{
	int	count;
	int	i;
	char	**temp;
	char	**new_envp;
	int x;
	int z;
	int exit_status;

	x = 1;
	exit_status = 0;
	temp = *envp;
	new_envp = NULL;
	count = get_array_len(command);
	if (count > 1)
	{
		while (command[x])
		{
			z = 0;
			i = 0;
			if (!ft_check_key_export(command[x]))
			{
				printf("zapshell: export: '%s': not a valid identifier\n", command[x]);
				exit_status = 1;
			}
			else if (ft_check_key_export(command[x]) == 1)
			{
				count = get_array_len(temp);
				new_envp = malloc(sizeof(char *) * (count + 2));
				while (count > i)
				{
					if (strncmp(command[x], temp[i], ft_strlen_env(command[x])))
					{
						new_envp[z] = strdup(temp[i]);
						z++;
					}
					i++;
				}
				new_envp[z] = strdup(command[x]);
				new_envp[z + 1] = NULL;
				count = 0;
				while (temp[count])
				{
					free(temp[count]);
					count++;
				}
				free(temp);
				temp = new_envp;
			}
			x++;
		}
	}
	else
		ft_print_env_sorted(*envp);
	if (new_envp)
		*envp = new_envp;
	return (exit_status);
}

static void ft_print_env_sorted(char **envp)
{
}

static int	ft_check_key_export(char *string)
{
	int	i;
	int len;

	i = 1;
	len = ft_strlen_env(string);
	if (len == strlen(string))
		return (2);
	if (!ft_isalpha(*string) && *string != '_')
		return (0);
	while (string[i] && string[i] != '=' && i < len)
	{
		if (!ft_isalnum(string[i]) && string[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
