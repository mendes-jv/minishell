/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:06:52 by pmelo-ca          #+#    #+#             */
/*   Updated: 2024/09/09 19:59:29 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	exec_simple_command(t_minishell **minishell, bool piped,
				t_ast *node);
static void	exec_child(t_minishell **minishell, bool piped, t_ast *node);
static void	exec_pipeline(t_minishell **minishell, t_ast *node);
static void	exec_pipe_child(t_minishell **minishell, int pipe_fd[2],
				char *pipe_direction, t_ast	*node);

void	execute_ast(t_minishell **minishell, bool piped, t_ast	*node)
{
	if (!node)
		return ;
	if (node->flag == PIPE)
		exec_pipeline(minishell, node);
	else if (node->flag == D_AND)
	{
		execute_ast(minishell, false, node->left);
		if (!(*minishell)->exit_status)
			execute_ast(minishell, false, node->right);
	}
	else if (node->flag == D_PIPE)
	{
		execute_ast(minishell, false, node->left);
		if ((*minishell)->exit_status)
			execute_ast(minishell, false, node->right);
	}
	else
		exec_simple_command(minishell, piped, node);
}

static void	exec_simple_command(t_minishell **minishell, bool piped,
	t_ast *node)
{
	if (!node->expanded_cmd)
	{
		check_redirection(minishell, piped, node);
		reset_redirects(piped, *minishell);
	}
	else if (is_builtin(node->expanded_cmd[0]))
	{
		check_redirection(minishell, piped, node);
		if ((*minishell)->exit_status)
			reset_redirects(piped, *minishell);
		else
		{
			(*minishell)->exit_status = builtin_exec(minishell, node);
			reset_redirects(piped, *minishell);
		}
	}
	else
		exec_child(minishell, piped, node);
}

static void	exec_child(t_minishell **minishell, bool piped, t_ast *node)
{
	int	pid_fork;

	pid_fork = fork();
	signals_interactive_parent();
	if (!pid_fork)
	{
		check_redirection(minishell, piped, node);
		if ((*minishell)->exit_status)
		{
			if (piped)
				exit(WEXITSTATUS((*minishell)->exit_status));
			exit((*minishell)->exit_status);
		}
		get_path(minishell, node);
		if (!(*minishell)->path)
			exit_handler(minishell, node);
		signals_interactive_child();
		if (execve((*minishell)->path, node->expanded_cmd,
				(*minishell)->env_copy) == -1)
			exit_handler(minishell, node);
	}
	waitpid(pid_fork, &(*minishell)->exit_status, 0);
	signals_non_interactive();
}

static void	exec_pipeline(t_minishell **minishell, t_ast *node)
{
	int	pipe_fd[2];
	int	pid_l;
	int	pid_r;

	pipe(pipe_fd);
	pid_l = fork();
	if (!pid_l)
		exec_pipe_child(minishell, pipe_fd, "LEFT", node->left);
	else
	{
		pid_r = fork();
		if (!pid_r)
			exec_pipe_child(minishell, pipe_fd, "RIGHT", node->right);
		else
		{
			close_fds(pipe_fd[0], pipe_fd[1]);
			waitpid(pid_r, &(*minishell)->exit_status, 0);
		}
	}
}

static void	exec_pipe_child(t_minishell **minishell, int pipe_fd[2],
		char *pipe_direction, t_ast	*node)
{
	int	status;

	if (!ft_strncmp(pipe_direction, "LEFT", 4))
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
	}
	else if (!ft_strncmp(pipe_direction, "RIGHT", 5))
	{
		close(pipe_fd[1]);
		dup2(pipe_fd[0], STDIN_FILENO);
		close(pipe_fd[0]);
	}
	execute_ast(minishell, true, node);
	status = (*minishell)->exit_status;
	clear_minishell(*minishell);
	exit(WEXITSTATUS(status));
}
