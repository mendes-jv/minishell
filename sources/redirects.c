/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:07:01 by pmelo-ca          #+#    #+#             */
/*   Updated: 2024/07/19 16:20:47 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	redirect_in(t_redir *tmp_redirs);
static int	redirect_out(t_redir *tmp_redirs);
static int	redirect_append(t_redir *tmp_redirs);
static void	redirect_heredoc(t_redir *tmp_redirs);

void	check_redirection(t_minishell **minishell)
{
	t_dlist	*dlist_redirs;
	t_redir	*temp_redir;

	dlist_redirs = (*minishell)->ast->redirs;
//	if (!dlist_redirs) //TODO: check this later; removing allowed exit with correct EE
//		(*minishell)->exit_status = 0;
	while (dlist_redirs)
	{
		temp_redir = (t_redir *)dlist_redirs->content;
		if (temp_redir->flag == GREATER)
			(*minishell)->exit_status = redirect_out(temp_redir);
		else if (temp_redir->flag == LESSER)
			(*minishell)->exit_status = redirect_in(temp_redir);
		else if (temp_redir->flag == D_GREATER)
			(*minishell)->exit_status = redirect_append(temp_redir);
		else if (temp_redir->flag == D_LESSER)
			redirect_heredoc(temp_redir);
		if ((*minishell)->exit_status)
			break ;
		dlist_redirs = dlist_redirs->next;
	}
}

static int	redirect_in(t_redir *tmp_redirs)
{
	int	fd;

	if (!tmp_redirs->expanded_values || !tmp_redirs->expanded_values[1])
		return (dprintf(2, ERROR_AMBIGUOUS_REDIRECT, tmp_redirs->value), 1);
	fd = open(tmp_redirs->expanded_values[0], O_RDONLY);
	if (fd == -1)
	{
		if (errno == EACCES)
			return (dprintf(2, ERROR_EXEC_INVALID_PATH, tmp_redirs->value), 1);
		return (dprintf(2, ERROR_EXEC_PERMISSION_DENY, tmp_redirs->value), 1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

static int	redirect_out(t_redir *tmp_redirs)
{
	int	fd;

	if (!tmp_redirs->expanded_values || !tmp_redirs->expanded_values[1])
		return (dprintf(2, ERROR_AMBIGUOUS_REDIRECT, tmp_redirs->value), 1);
	fd = open(tmp_redirs->expanded_values[0], O_CREAT | O_WRONLY | O_TRUNC,
			S_IRWXU | S_IRWXG | S_IRWXO);
	if (fd == -1)
		return (dprintf(2, ERROR_EXEC_PERMISSION_DENY, tmp_redirs->value), 1);
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

static int	redirect_append(t_redir *tmp_redirs)
{
	int	fd;

	if (!tmp_redirs->expanded_values || !tmp_redirs->expanded_values[1])
		return (dprintf(2, ERROR_AMBIGUOUS_REDIRECT, tmp_redirs->value), 1);
	fd = open(tmp_redirs->expanded_values[0], O_CREAT | O_WRONLY | O_APPEND,
			S_IRWXU | S_IRWXG | S_IRWXO);
	if (fd == -1)
		return (dprintf(2, ERROR_EXEC_PERMISSION_DENY, tmp_redirs->value), 1);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

static void	redirect_heredoc(t_redir *tmp_redirs)
{
	dup2(tmp_redirs->heredoc, STDIN_FILENO);
	close(tmp_redirs->heredoc);
}
