/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 17:26:15 by pmelo-ca          #+#    #+#             */
/*   Updated: 2024/08/20 17:35:35 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_strjoind(char *first, char *second, char *delimiter)
{
	char	*string;
	size_t	length;
	size_t	allocation_length;

	if (!first || !second)
		return (NULL);
	if (!ft_strlen(first) || !ft_strlen(second))
		return (ft_strjoin(first, second));
	length = ft_strlen(first);
	allocation_length = length + ft_strlen(second) + ft_strlen(delimiter) + 1;
	string = calloc(allocation_length, sizeof(char));
	if (!string)
		return (NULL);
	ft_strlcpy(string, first, length + 1);
	while (*delimiter)
		string[length++] = *delimiter++;
	while (*second)
		string[length++] = *second++;
	string[length] = '\0';
	return (string);
}

bool	join_command(char **cmd, t_dlist **word)
{
	char	*old_cmd;

	if (!*cmd)
		*cmd = ft_strdup("");
	if (!*cmd)
		return (false);
	while (*word && is_flag((*word)->content, WORD))
	{
		old_cmd = *cmd;
		*cmd = ft_strjoind(*cmd, ((t_token *)(*word)->content)->value,
				WHITESPACE);
		free(old_cmd);
		if (!*cmd)
			return (false);
		*word = (*word)->next;
	}
	return (true);
}

bool	append_redir(t_dlist **redirs, t_dlist **words,
			t_parse_status *status)
{
	t_redir	*temp_redir;
	char	*dup_value;
	t_flag	flag;

	while (*words && is_redir((*words)->content))
	{
		flag = ((t_token *)(*words)->content)->flag;
		*words = (*words)->next;
		if (!*words || !is_flag((*words)->content, WORD))
			return (set_parse_status(status, SYNTAX_ERROR, *words), false);
		dup_value = ft_strdup(((t_token *)(*words)->content)->value);
		if (!dup_value)
			return (set_parse_status(status, SYNTAX_ERROR, *words), false);
		temp_redir = calloc(1, sizeof(t_redir));
		*temp_redir = (t_redir){dup_value, NULL, 0, flag};
		ft_dlstadd_b(redirs, ft_dlstnew((temp_redir)));
		*words = (*words)->next;
	}
	return (true);
}

void	set_parse_status(t_parse_status *status,
	enum e_parse_status new_status, t_dlist *word)
{
	t_flag	flag;

	if (!word)
		flag = END;
	else
		flag = ((t_token *)word->content)->flag;
	*status = (t_parse_status){new_status, flag};
}

void	manage_error_status(t_parse_status status, t_minishell **minishell)
{
	if (status.current == SYNTAX_ERROR)
		ft_putstr_fd("zapshell: syntax error unexpected token \n",
			STDERR_FILENO);
	if (status.current == INVALID_ERROR)
	{
		(*minishell)->ast = NULL;
		ft_putstr_fd("zapshell: input error\n", STDERR_FILENO);
	}
	if (status.current == MEMORY_ERROR)
		ft_putstr_fd("zapshell: memory error\n", STDERR_FILENO);
	if (status.current == HEREDOC_ERROR)
	{
		(*minishell)->exit_status = 130;
		return ;
	}
	(*minishell)->exit_status = 2;
}
