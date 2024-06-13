#include "../../includes/minishell.h"

static int	ft_check_key_export(char *string);
static void ft_print_env_sorted(char **envp);
static char *ft_get_lowest_alpha_env(t_list **ordered_list);
static void *ft_remove_node(t_list *node);
static void ft_list_remove_if(t_list **begin_list, void *data_ref);
static int ft_check_node_asc_order(t_list *node, t_list *list_node);

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
				new_envp[z] = strdup(command[x]); //validar aspas na string a partir do =
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
    t_list *ordered_list;
    int i;
    char **temp;

    i = 0;
    ordered_list = NULL;
    while (envp[i])
    {
        ft_lstadd_back(&ordered_list, ft_lstnew((void*)envp[i]));
        i++;
    }
    temp = malloc(sizeof (char *) * ft_lstsize(ordered_list) + 1);
    if(!temp)
        return;
    i = 0;
    while (ft_lstsize(ordered_list) > 0)
    {
        temp[i] = ft_get_lowest_alpha_env(&ordered_list);
        i++;
    }
    temp[i] = NULL;
    i = 0;
    while(temp[i])
    {
        printf("declare -x \"%s\"\n", temp[i]);
        free(temp[i]);
        i++;
    }
    free(temp);
}

static char *ft_get_lowest_alpha_env(t_list **list)
{
    char *temp_str;
    void *node;
    t_list *temp_lst;

    temp_str = NULL;
    temp_lst = *list;
    node = temp_lst->content;
    while(temp_lst)
    {
        if (ft_strncmp(node, temp_lst->content, ft_strlen(node)) < 0)
            node = temp_lst->content;
        temp_lst = temp_lst->next;
    }
    ft_list_remove_if(list, node);
    temp_str = strdup(node);
    return (temp_str);
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

static void ft_list_remove_if(t_list **begin_list, void *data_ref)
{
    t_list *cur;

    if (begin_list == NULL || *begin_list == NULL)
        return;
    cur = *begin_list;
    if (!ft_strncmp(cur->content, data_ref, ft_strlen(data_ref)))
    {
        *begin_list = cur->next;
        free(cur);
    }
    else
    {
        cur = *begin_list;
        ft_list_remove_if(&cur->next, data_ref);
    }
}
