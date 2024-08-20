/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:06:52 by pmelo-ca          #+#    #+#             */
/*   Updated: 2024/08/19 17:19:01 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	exec_simple_command(t_minishell **minishell, bool piped);
static void	exec_child(t_minishell **minishell);
static void	exec_pipeline(t_minishell **minishell);
static void	exec_pipe_child(t_minishell **minishell, int pipe_fd[2],
				char *pipe_direction);

void	execute_ast(t_minishell **minishell, bool piped)
{
	if (!(*minishell)->ast)
		(*minishell)->exit_status = 1;
	if ((*minishell)->ast->flag == PIPE)
		exec_pipeline(minishell);
	else if ((*minishell)->ast->flag == D_AND)
	{
		(*minishell)->ast = (*minishell)->ast->left;
		execute_ast(minishell, false);
		if (!(*minishell)->exit_status)
		{
			(*minishell)->ast = (*minishell)->ast->right;
			execute_ast(minishell, false);
		}
	}
	else if ((*minishell)->ast->flag == D_PIPE)
	{
		(*minishell)->ast = (*minishell)->ast->left;
		execute_ast(minishell, false);
		if ((*minishell)->exit_status)
		{
			(*minishell)->ast = (*minishell)->ast->right;
			execute_ast(minishell, false);
		}
	}
	else
		exec_simple_command(minishell, piped);
}

static void	exec_simple_command(t_minishell **minishell, bool piped)
{
	if (!(*minishell)->ast->expanded_cmd)
	{
		check_redirection(minishell);
		reset_redirects(piped, *minishell);
	}
	else if (is_builtin((*minishell)->ast->expanded_cmd[0]))
	{
		check_redirection(minishell);
		//		if ((*minishell)->exit_status)
		//		{
		//			reset_redirects(piped);
		//TODO: check this later; removing allowed exit with correct EE
		//			return ;
		//		}
		(*minishell)->exit_status = builtin_exec(minishell);
		reset_redirects(piped, *minishell);
	}
	else
		exec_child(minishell);
}

static void	exec_child(t_minishell **minishell)
{
	int	pid_fork;

	pid_fork = fork();
	if (!pid_fork)
	{
		check_redirection(minishell);
		if ((*minishell)->exit_status)
			return ;
		get_path(minishell);
		if (!(*minishell)->path)
			exit_handler(ERROR_EXEC_COM_NOT_FOUND,(*minishell)->ast->expanded_cmd[0],
						 minishell, true, 127);
		if (!ft_strncmp((*minishell)->path, "invalid", 8))
			exit_handler(ERROR_EXEC_INVALID_PATH,(*minishell)->ast->expanded_cmd[0],
						 minishell, true, 127);
		if (execve((*minishell)->path, (*minishell)->ast->expanded_cmd,
				(*minishell)->env_copy) == -1)
			exit_handler(NULL, NULL, minishell, false, 1);
	}
	waitpid(pid_fork, &(*minishell)->exit_status, 0);
}

static void	exec_pipeline(t_minishell **minishell)
{
	int	pipe_fd[2];
	int	pid_l;
	int	pid_r;

	pipe(pipe_fd);
	pid_l = fork();
	if (!pid_l)
		exec_pipe_child(minishell, pipe_fd, "LEFT");
	else
	{
		pid_r = fork();
		if (!pid_r)
			exec_pipe_child(minishell, pipe_fd, "RIGHT");
		else
		{
			waitpid(pid_l, &(*minishell)->exit_status, 0);
			waitpid(pid_r, &(*minishell)->exit_status, 0);
			close(pipe_fd[0]);
			close(pipe_fd[1]);
			(*minishell)->exit_status /= 256;
		}
	}
}

static void	exec_pipe_child(t_minishell **minishell, int pipe_fd[2],
		char *pipe_direction)
{
	if (!ft_strncmp(pipe_direction, "LEFT", 4))
	{
		(*minishell)->ast = (*minishell)->ast->left;
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
	}
	else if (!ft_strncmp(pipe_direction, "RIGHT", 5))
	{
		(*minishell)->ast = (*minishell)->ast->right;
		close(pipe_fd[1]);
		dup2(pipe_fd[0], STDIN_FILENO);
		close(pipe_fd[0]);
	}
	execute_ast(minishell, true);
	clear_minishell(*minishell);
	exit((*minishell)->exit_status);
}
