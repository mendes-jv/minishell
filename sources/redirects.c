/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:07:01 by pmelo-ca          #+#    #+#             */
/*   Updated: 2024/09/10 17:14:26 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	redirect_in(t_redir *tmp_redirs, bool piped);
static int	redirect_out(t_redir *tmp_redirs, bool piped);
static int	redirect_append(t_redir *tmp_redirs, bool piped);
static void	redirect_heredoc(t_redir *tmp_redirs);

void	check_redirection(t_minishell **minishell, bool piped, t_ast *node)
{
	t_dlist	*dlist_redirs;
	t_redir	*temp_redir;

	dlist_redirs = node->redirs;
	if (!dlist_redirs)
		exit_check(minishell, node);
	while (dlist_redirs)
	{
		temp_redir = (t_redir *)dlist_redirs->content;
		if (temp_redir->flag == GREATER)
			(*minishell)->exit_status = redirect_out(temp_redir, piped);
		else if (temp_redir->flag == LESSER)
			(*minishell)->exit_status = redirect_in(temp_redir, piped);
		else if (temp_redir->flag == D_GREATER)
			(*minishell)->exit_status = redirect_append(temp_redir, piped);
		else if (temp_redir->flag == D_LESSER)
			redirect_heredoc(temp_redir);
		if ((*minishell)->exit_status)
			break ;
		dlist_redirs = dlist_redirs->next;
	}
}

static int	redirect_in(t_redir *tmp_redirs, bool piped)
{
	int	fd;

	if (!tmp_redirs->expanded_values || !tmp_redirs->expanded_values[0])
		return (dprintf(2, ERROR_AMBIGUOUS_REDIRECT, tmp_redirs->value), 1);
	fd = open(tmp_redirs->expanded_values[0], O_RDONLY);
	if (fd == -1)
	{
		if (errno == ENOENT)
		{
			if (piped)
				return (dprintf(2, ERROR_EXEC_INVALID_PATH, tmp_redirs->value),
					257);
			return (dprintf(2, ERROR_EXEC_INVALID_PATH, tmp_redirs->value), 1);
		}
		if (piped)
			return (dprintf(2, ERROR_EXEC_PERMISSION_DENY, tmp_redirs->value),
				257);
		return (dprintf(2, ERROR_EXEC_PERMISSION_DENY, tmp_redirs->value), 1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

static int	redirect_out(t_redir *tmp_redirs, bool piped)
{
	int	fd;

	if (!tmp_redirs->expanded_values || !tmp_redirs->expanded_values[0])
		return (dprintf(2, ERROR_AMBIGUOUS_REDIRECT, tmp_redirs->value), 1);
	fd = open(tmp_redirs->expanded_values[0], O_CREAT | O_WRONLY | O_TRUNC,
			S_IRWXU | S_IRWXG | S_IRWXO);
	if (fd == -1)
	{
		if (piped)
			return (dprintf(2, ERROR_EXEC_PERMISSION_DENY, tmp_redirs->value),
				257);
		return (dprintf(2, ERROR_EXEC_PERMISSION_DENY, tmp_redirs->value), 1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

static int	redirect_append(t_redir *tmp_redirs, bool piped)
{
	int	fd;

	if (!tmp_redirs->expanded_values || !tmp_redirs->expanded_values[0])
		return (dprintf(2, ERROR_AMBIGUOUS_REDIRECT, tmp_redirs->value), 1);
	fd = open(tmp_redirs->expanded_values[0], O_CREAT | O_WRONLY | O_APPEND,
			S_IRWXU | S_IRWXG | S_IRWXO);
	if (fd == -1)
	{
		if (piped)
			return (dprintf(2, ERROR_EXEC_PERMISSION_DENY, tmp_redirs->value),
				257);
		return (dprintf(2, ERROR_EXEC_PERMISSION_DENY, tmp_redirs->value), 1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

static void	redirect_heredoc(t_redir *tmp_redirs)
{
	dup2(tmp_redirs->heredoc, STDIN_FILENO);
	close(tmp_redirs->heredoc);
}
