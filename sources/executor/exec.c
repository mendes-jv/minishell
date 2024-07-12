#include "../../includes/minishell.h"

static int exec_simple_command(t_ast *ast, bool piped);
static int exec_child(t_ast *ast);
static int exec_pipeline(t_ast *ast);
static void exec_pipe_child(t_ast *ast, int pipe_fd[2], char *pipe_direction);

int		 execute_ast(t_ast *ast, bool piped)
{
	int exit_status;

	if (!ast)
		return (1);
	if (ast->flag == WORD)
		return(exec_simple_command(ast, piped));
	else if (ast->flag == PIPE)
		return(exec_pipeline(ast));
	else if (ast->flag == D_AND)
	{
		exit_status = execute_ast(ast->left, false);
		if (!exit_status)
			return(execute_ast(ast->right, false));
		return(exit_status);
	}
	else if (ast->flag == D_PIPE)
	{
		exit_status = execute_ast(ast->left, false);
		if (exit_status)
			return(exit_status);
		return(execute_ast(ast->right, false));
	}
	return (exit_status);
}

static int exec_simple_command(t_ast *ast, bool piped)
{
	int exit_status;

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
		exit_status = builtin_exec(ast->expanded_cmd, envp)); // TODO validate env
		return (exit_status);
	}
	else
		return (exec_child(ast));
}

static int exec_child(t_ast *ast)
{
	int		pid_fork;
	int 	exit_status;
	char	*path;

	pid_fork = fork();
	if (!pid_fork)
	{
		exit_status = check_redirection(ast);
		if (!exit_status)
			return (exit_status);
		path = get_path(ast->expanded_cmd[0], env); //TODO create solution to free path
		if (!path)
			exit_status = exec_error_handler(127, "command not found\n", ast->expanded_cmd[0]); // TODO must clean mm allocated
		if (!ft_strncmp(path, "invalid", 8))
			exit_status = exec_error_handler(127, "No such file or directory\n", ast->expanded_cmd[0]); //TODO must clean mm allocated
		if (execve(path, ast->expanded_cmd, env) == -1)
			exit(1); // TODO must clean mm allocated
	}
	waitpid(pid_fork, &exit_status, 0);
	return (exit_status / 256);
}

static int exec_pipeline(t_ast *ast)
{
	int exit_status;
	int pipe_fd[2];
	int pid_l;
	int pid_r;

	pipe(pipe_fd);
	pid_l = fork();
	if (!pid_l)
		exec_pipe_child(ast->left, pipe_fd, "LEFT");
	else
	{
		pid_r = fork();
		if (!pid_r)
			exec_pipe_child(ast->right, pipe_fd, "RIGHT");
		else
		{
			waitpid(pid_l, &exit_status, 0);
			waitpid(pid_r, &exit_status, 0);
			close(pipe_fd[0]);
			close(pipe_fd[1]);
			return (exit_status / 256);
		}
	}
}

static void exec_pipe_child(t_ast *ast, int pipe_fd[2], char *pipe_direction)
{
	int exit_status;

	if(!ft_strncmp(pipe_direction, "LEFT", 4))
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
	}
	else if(!ft_strncmp(pipe_direction, "RIGHT", 5))
	{
		close(pipe_fd[1]);
		dup2(pipe_fd[0], STDIN_FILENO);
		close(pipe_fd[0]);
	}
	exit_status = execute_ast(ast, true);
	exit(exit_status); // TODO must clean mm allocated
}
