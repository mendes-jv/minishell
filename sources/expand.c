/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 17:09:24 by pmelo-ca          #+#    #+#             */
/*   Updated: 2024/09/02 17:04:23 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	expand_redir(t_redir *redir, t_minishell **minishell);
static void	heredoc(char *value, pid_t *pipe_fds, t_minishell **minishell);
static bool	is_delimiter(char *doc_line, char *values);
static void	dlstiter_redir(t_dlist *lst, void (*f)(void *, t_minishell **),
				t_minishell **minishell);

void	expand(t_ast **ast, t_minishell **minishell, t_parse_status *status)
{
	t_token	flag;

	if (!*ast)
		return ;
	flag.flag = (*ast)->flag;
	if (is_binary_operator(&flag))
	{
		expand(&(*ast)->left, minishell, status);
		//TODO: if !heredoc_sigint
		expand(&(*ast)->right, minishell, status);
	}
	else
	{
		if ((*ast)->cmd)
			(*ast)->expanded_cmd = expand_string((*ast)->cmd, minishell);
		dlstiter_redir((*ast)->redirs, (void (*)(void *,
					t_minishell **))expand_redir, minishell);
	}
}

char	**expand_string(char *cmd, t_minishell **minishell)
{
	char	**expanded_cmd;
	char	**quoted_cmd;
	int		i;

	i = 0;
	cmd = clean_string(cmd, (*minishell)->env_copy, (*minishell)->exit_status);
	if (!cmd)
		return (NULL);
	cmd = handle_empty_cmd_strings(cmd);
	if (!cmd)
		return (NULL);
	expanded_cmd = split_cmd(cmd);
	free(cmd);
	if (!expanded_cmd)
		return (NULL);
	quoted_cmd = ft_calloc(sizeof(char *), get_array_len(expanded_cmd) + 1);
	if (!quoted_cmd)
		return (NULL);
	while (i < get_array_len(expanded_cmd))
	{
		quoted_cmd[i] = strip_quotes(expanded_cmd[i]);
		i++;
	}
	quoted_cmd[i] = NULL;
	free(expanded_cmd);
	return (quoted_cmd);
}

static void	expand_redir(t_redir *redir, t_minishell **minishell)
{
	pid_t	pipe_fds[2];
	pid_t	pid;

	//TODO:	bool	child_sigint;
	if (redir->flag == D_LESSER)
	{
		pipe(pipe_fds);
		//TODO: child_sigint = false;
		//signal(SIGQUIT, SIG_IGN);
		pid = fork();
		if (!pid)
			heredoc(redir->value, pipe_fds, minishell);
		waitpid(pid, &pid, 0);
		//TODO:	signal(SIGQUIT, sigquit_handler);
		//TODO: child_sigint == true
		close(pipe_fds[1]);
		if (WIFEXITED(pid) && WEXITSTATUS(pid) == SIGINT)
			return ;
		redir->heredoc = pipe_fds[0];
	}
	else
		redir->expanded_values = expand_string(redir->value, minishell);
}

void	dlstiter_redir(t_dlist *lst, void (*f)(void *, t_minishell **),
		t_minishell **minishell)
{
	t_dlist	*temp_node;

	temp_node = lst;
	if (!lst || !f)
		return ;
	while (temp_node)
	{
		f(temp_node->content, minishell);
		temp_node = temp_node->next;
	}
}

static void	heredoc(char *value, pid_t *pipe_fds, t_minishell **minishell)
{
	char	*doc_line;
	char	*quote_or_null;

	//	signal(SIGINT, heredoc_sigint);
	quote_or_null = value;
	while (*quote_or_null && !ft_strchr(QUOTES, *quote_or_null))
		quote_or_null++;
	doc_line = readline(HEREDOC_PROMPT);
	while (doc_line && !is_delimiter(doc_line, value))
	{
		if (!*quote_or_null)
			expand_heredoc(doc_line, pipe_fds[1], minishell);
		else
			ft_putendl_fd(doc_line, pipe_fds[1]);
		free(doc_line);
		doc_line = readline(HEREDOC_PROMPT);
	}
	if (doc_line)
		free(doc_line);
	else
		printf(HEREDOC_WARNING);
	clear_minishell(*minishell);
	exit(EXIT_SUCCESS);
}

bool	is_delimiter(char *doc_line, char *value)
{
	while (*doc_line)
	{
		while (*value && ft_strchr(QUOTES, *value))
			value++;
		if (*doc_line == *value)
		{
			doc_line++;
			value++;
		}
		else
			return (false);
	}
	while (*value && ft_strchr(QUOTES, *value))
		value++;
	return (!*value);
}
