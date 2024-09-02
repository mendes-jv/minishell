/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:07:24 by pmelo-ca          #+#    #+#             */
/*   Updated: 2024/09/02 16:58:28 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	check_key_export(char *string);
static void	print_env_sorted(char **envp);
static char	*get_lowest_alpha_env(t_list **ordered_list);
static char	**new_env_export(char *command, char **old_env);

int	exec_export(char **command, char ***env)
{
	char	**new_env;
	int		x;
	int		exit_status;

	x = 1;
	exit_status = 0;
	new_env = NULL;
	if (get_array_len(command) > 1)
	{
		while (command[x])
		{
			if (!check_key_export(command[x]))
				exit_status = error_handler(1, 2, ERROR_EXPORT_ID, command[x]);
			else if (check_key_export(command[x]))
			{
				new_env = new_env_export(command[x], *env);
				*env = new_env;
			}
			x++;
		}
	}
	else
		print_env_sorted(*env);
	return (exit_status);
}

static char	**new_env_export(char *command, char **old_env)
{
	int		count;
	char	**new_env;
	int		z;
	int		i;

	z = 0;
	i = 0;
	count = get_array_len(old_env);
	new_env = calloc(sizeof(char *), count + 2);
	while (count > i)
	{
		if (ft_strncmp(command, old_env[i], strlen_env(command)))
		{
			new_env[z] = ft_strdup(old_env[i]);
			z++;
		}
		i++;
	}
	new_env[z] = ft_strdup(command);
	new_env[z + 1] = NULL;
	clear_matrix(old_env);
	return (new_env);
}

static void	print_env_sorted(char **env)
{
	t_list	*ordered_list;
	int		i;
	char	**temp;

	i = 0;
	ordered_list = NULL;
	while (env[i])
	{
		ft_lstadd_back(&ordered_list, ft_lstnew((void *)env[i]));
		i++;
	}
	temp = calloc(sizeof(char *), ft_lstsize(ordered_list) + 1);
	if (!temp)
		return ;
	i = 0;
	while (ft_lstsize(ordered_list) > 0)
		temp[i++] = get_lowest_alpha_env(&ordered_list);
	temp[i] = NULL;
	i = 0;
	while (temp[i])
		dprintf(1, "declare -x \"%s\"\n", temp[i++]);
	clear_matrix(temp);
}

static char	*get_lowest_alpha_env(t_list **list)
{
	char	*temp_str;
	void	*node;
	t_list	*temp_lst;

	temp_str = NULL;
	temp_lst = *list;
	node = temp_lst->content;
	while (temp_lst)
	{
		if (ft_strncmp(node, temp_lst->content, ft_strlen(node)) > 0)
			node = temp_lst->content;
		temp_lst = temp_lst->next;
	}
	list_remove_if(list, node);
	temp_str = ft_strdup(node);
	return (temp_str);
}

static int	check_key_export(char *string) //TODO: fix export A-
{
	size_t	i;
	size_t	len;

	i = 1;
	len = strlen_env(string);
	if (!ft_isalpha(string[0]))
		return (0);
	if (len == ft_strlen(string))
		return (2);
	if (!ft_isalpha(string[len]) && string[len] != '_' && string[len] != '=')
		return (0);
	while (string[i] && string[i] != '=' && i < len)
	{
		if (!ft_isalnum(string[i]) && string[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
