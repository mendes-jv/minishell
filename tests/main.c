#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

typedef struct s_list
{
    void			*content;
    struct s_list	*next;
}					t_list;

int	exec_export(char **command, char ***envp);
void	exec_unset(char **command, char ***envp);
static char **ft_unset_env(char *command, char **envp);
char	**ft_get_env(char **envp);
int	exec_env(char **command, char **envp);
int     get_array_len(char **arr);
static int	ft_islower(int c);
static int	ft_isupper(int c);
int	ft_isalpha(int c);
int	ft_isalnum(int c);
int	ft_isdigit(int c);
static int ft_strlen_env(char *command);
static int	ft_check_key_export(char *string);
char **ft_unset_env(char *command, char **envp);
static void ft_print_env_alpha_order(char **envp);
static void ft_print_env_sorted(char **envp);
static char *ft_get_lowest_alpha_env(t_list **list);
static void ft_list_remove_if(t_list **begin_list, void *data_ref);
void	ft_lstadd_back(t_list **lst, t_list *new);
t_list	*ft_lstnew(void *content);
t_list	*ft_lstlast(t_list *lst);
int	ft_lstsize(t_list *lst);


int main(int argc, char **argv, char **envp)
{
	char **envp_cp = NULL;
	char **arg = NULL;

	arg = malloc(sizeof (char *) * 4);
	arg[0] = strdup("export");
    arg[1] = strdup("obi=\'aaaaaa\'");
    arg[2] = strdup("obia=\"aaaaaa\"");
	envp_cp = ft_get_env(envp);
	exec_env(argv, envp_cp);
	printf("\n\n\n");
	exec_export(arg, &envp_cp);
    exec_env(argv, envp_cp);
	int count = 0;
	while (envp_cp[count])
	{
		free(envp_cp[count]);
		count++;
	}
	free(envp_cp);
}
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
        if (strncmp(node, temp_lst->content, strlen(node)) > 0)
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
    if (!strncmp(cur->content, data_ref, strlen(data_ref)))
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


int     get_array_len(char **arr)
{
	int arr_size;

	arr_size = 0;
	while (arr[arr_size])
		arr_size++;
	return (arr_size);
}

int	exec_env(char **command, char **envp)
{
	int	count;

	count = 0;
	if (get_array_len(command) > 1)
	{
		if (command[1][0] == '-')
			return (printf("env: invalid option -- '%c'\n", command[1][1]), 125);
		else
			return (printf("env: '%s': No such file or directory\n", command[1]), 127);
	}
	while (envp[count])
	{
		printf("%s\n", envp[count]);
		count++;
	}
	return 0;
}

char	**ft_get_env(char **envp)
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
		temp[i] = strdup(envp[i]);
		i++;
	}
	return(temp);
}

static int ft_strlen_env(char *command)
{
	int i;

	i = 0;
	while (command[i] && command[i] != '=')
		i++;
	return (i);
}



static int	ft_islower(int c)
{
	return (c >= 'a' && c <= 'z');
}

static int	ft_isupper(int c)
{
	return (c >= 'A' && c <= 'Z');
}

int	ft_isalpha(int c)
{
	return (ft_islower(c) || ft_isupper(c));
}

int	ft_isalnum(int c)
{
	return (ft_isalpha(c) || ft_isdigit(c));
}

int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

void	ft_lstadd_back(t_list **lst, t_list *new)
{
    t_list	*node;

    if (!*lst)
    {
        *lst = new;
        return ;
    }
    node = ft_lstlast(*lst);
    node->next = new;
}

t_list	*ft_lstnew(void *content)
{
    t_list	*node;

    node = malloc(sizeof(t_list));
    if (!node)
        return (NULL);
    node->content = content;
    node->next = NULL;
    return (node);
}

t_list	*ft_lstlast(t_list *lst)
{
    t_list	*node;

    node = lst;
    if (lst == NULL)
        return (NULL);
    while (node->next)
        node = node->next;
    return (node);
}

int	ft_lstsize(t_list *lst)
{
    int		index;

    index = 0;
    while (++index && lst)
        lst = lst->next;
    return (--index);
}
