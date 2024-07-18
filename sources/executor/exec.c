/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:06:52 by pmelo-ca          #+#    #+#             */
/*   Updated: 2024/07/18 14:10:16 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	exec_simple_command(t_ast *ast, bool piped, char ***env);
static int	exec_child(t_ast *ast, char ***env);
static int	exec_pipeline(t_ast *ast, char ***env);
static void	exec_pipe_child(t_ast *ast, int pipe_fd[2], char *pipe_direction,
				char ***env);

int	execute_ast(t_ast *ast, bool piped, char ***env)
{
	int	exit_status;

	exit_status = 0;
	if (!ast)
		return (1);
	if (ast->flag == PIPE)
		return (exec_pipeline(ast, env));
	else if (ast->flag == D_AND)
	{
		exit_status = execute_ast(ast->left, false, env);
		if (!exit_status)
			return (execute_ast(ast->right, false, env));
		return (exit_status);
	}
	else if (ast->flag == D_PIPE)
	{
		exit_status = execute_ast(ast->left, false, env);
		if (exit_status)
			return (exit_status);
		return (execute_ast(ast->right, false, env));
	}
	else
		return (exec_simple_command(ast, piped, env));
}

static int	exec_simple_command(t_ast *ast, bool piped, char ***env)
{
	int	exit_status;

	exit_status = 0;
	if (!ast->expanded_cmd)
	{
		exit_status = check_redirection(ast);
		reset_redirects(piped);
		return (exit_status);
	}
	else if (is_builtin(ast->expanded_cmd[0]))
	{
		exit_status = check_redirection(ast);
		if (exit_status)
		{
			reset_redirects(piped);
			return (exit_status);
		}
		reset_redirects(piped);
		exit_status = builtin_exec(ast->expanded_cmd, env);
		return (exit_status);
	}
	else
		return (exec_child(ast, env));
}

static int	exec_child(t_ast *ast, char ***env)
{
	int		pid_fork;
	int		exit_status;
	char	*path;

	pid_fork = fork();
	if (!pid_fork)
	{
		exit_status = check_redirection(ast);
		if (!exit_status)
			return (exit_status);
		path = get_path(ast->expanded_cmd[0], *env);//TODO create solution to free path;
		if (!path)
		{
			dprintf(2, ERROR_EXEC_COM_NOT_FOUND, ast->expanded_cmd[0]);
			clear_ast(ast);
			exit(127);
		}
		if (!ft_strncmp(path, "invalid", 8))
		{
			dprintf(2, ERROR_EXEC_INVALID_PATH, ast->expanded_cmd[0]);
			clear_ast(ast);
			exit(127);
		}
		if (execve(path, ast->expanded_cmd, *env) == -1)
		{
			clear_ast(ast);
			exit(1);
		}
	}
	waitpid(pid_fork, &exit_status, 0);
	return (exit_status / 256);
}

static int	exec_pipeline(t_ast *ast, char ***env)
{
	int	exit_status;
	int	pipe_fd[2];
	int	pid_l;
	int	pid_r;

	pipe(pipe_fd);
	pid_l = fork();
	if (!pid_l)
		exec_pipe_child(ast->left, pipe_fd, "LEFT", env);
	else
	{
		pid_r = fork();
		if (!pid_r)
			exec_pipe_child(ast->right, pipe_fd, "RIGHT", env);
		else
		{
			waitpid(pid_l, &exit_status, 0);
			waitpid(pid_r, &exit_status, 0);
			close(pipe_fd[0]);
			close(pipe_fd[1]);
			return (exit_status / 256);
		}
	}
	return (-1);
}

static void	exec_pipe_child(t_ast *ast, int pipe_fd[2], char *pipe_direction,
		char ***env)
{
	int	exit_status;

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
	exit_status = execute_ast(ast, true, env);
	clear_ast(ast);
	exit(exit_status);
}
