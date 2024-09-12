/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 17:09:24 by pmelo-ca          #+#    #+#             */
/*   Updated: 2024/09/12 14:02:55 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	expand_redir(t_redir *redir, t_minishell **minishell,
				t_parse_status *status);
static void	heredoc(char *value, pid_t *pipe_fds, t_minishell **minishell);
void		dlstiter_redir(t_dlist *lst, void (*f)(void *, t_minishell **,
					t_parse_status *), t_minishell **minishell,
				t_parse_status *status);

void	expand(t_ast **ast, t_minishell **minishell, t_parse_status *status)
{
	t_token	flag;

	if (!*ast)
		return ;
	flag.flag = (*ast)->flag;
	if (is_binary_operator(&flag))
	{
		expand(&(*ast)->left, minishell, status);
		expand(&(*ast)->right, minishell, status);
	}
	else
	{
		if ((*ast)->cmd)
			(*ast)->expanded_cmd = expand_string((*ast)->cmd, minishell);
		dlstiter_redir((*ast)->redirs, (void (*)(void *, t_minishell **,
					t_parse_status *))expand_redir, minishell, status);
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

static void	expand_redir(t_redir *redir, t_minishell **minishell,
		t_parse_status *status)
{
	pid_t	pipe_fds[2];
	pid_t	pid;

	if (redir->flag == D_LESSER)
	{
		pipe(pipe_fds);
		pid = fork();
		if (!pid)
			heredoc(redir->value, pipe_fds, minishell);
		signals_heredoc_parent();
		waitpid(pid, &pid, 0);
		close(pipe_fds[1]);
		signals_non_interactive();
		tcsetattr(STDIN_FILENO, TCSANOW, (*minishell)->original_term);
		if (pid != 0)
		{
			status->current = HEREDOC_ERROR;
			return ;
		}
		(*minishell)->exit_status = 0;
		redir->heredoc = pipe_fds[0];
	}
	else
		redir->expanded_values = expand_string(redir->value, minishell);
}

void	dlstiter_redir(t_dlist *lst, void (*f)(void *, t_minishell **,
			t_parse_status *), t_minishell **minishell, t_parse_status *status)
{
	t_dlist	*temp_node;

	temp_node = lst;
	if (!lst || !f)
		return ;
	while (temp_node)
	{
		f(temp_node->content, minishell, status);
		temp_node = temp_node->next;
	}
}

static void	heredoc(char *value, pid_t *pipe_fds, t_minishell **minishell)
{
	char	*doc_line;
	char	*quote_or_null;
	int		status;

	status = 0;
	signals_heredoc_child();
	quote_or_null = value;
	while (*quote_or_null && !ft_strchr(QUOTES, *quote_or_null))
		quote_or_null++;
	while (true)
	{
		doc_line = readline(HEREDOC_PROMPT);
		if (doc_line != NULL)
		{
			if (is_delimiter(doc_line, value))
				break ;
			if (!*quote_or_null)
				expand_heredoc(doc_line, pipe_fds[1], minishell);
			else
				ft_putendl_fd(doc_line, pipe_fds[1]);
			free(doc_line);
		}
		else
		{
			if (g_signal == 0)
				printf(HEREDOC_WARNING);
			else
				status = SIGINT;
			break ;
		}
	}
	close_fds(pipe_fds[0], pipe_fds[1]);
	g_signal = 0;
	clear_minishell(*minishell);
	exit(status);
}
