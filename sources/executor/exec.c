#include "../../includes/minishell.h"

static int exec_child(t_ast *ast);
static char *get_path(char *command, char **env);
static char	*validate_access(char *command);
static int exec_pipeline(t_ast *ast);
static void exec_pipe_child(t_ast *ast, int pipe_fd[2], char *pipe_direction);

int		execute_ast(t_ast *ast, bool piped)
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
			return(execute_ast(ast->right, false)));
		return(exit_status);
	}
	else if (ast->flag == D_PIPE)
	{
		exit_status = execute_ast(ast->left, false);
		if (exit_status)
			return(exit_status);
		return(execute_ast(ast->right, false)));
	}
	return (exit_status);
}

int	exec_simple_command(t_ast *ast, bool piped) // must implement redirect
{
	if (is_builtin(ast->expanded_cmd[0]))
		return (builtin_exec(ast->expanded_cmd, envp)); // validate env; fix builtin return
	else
		return (exec_child(ast));
}

static int exec_child(t_ast *ast)
{
	int pid;
	int exit_status;
	char	*path;

	pid = fork();
	if (!pid) //son
	{
		path = get_path(ast->expanded_cmd[0], env);
		if (!path)
			error_handler(127, s_pipex, cmd, argv); // handler exit_status
		if (!ft_strncmp(path, "invalid", 8))
			error_handler(5, s_pipex, cmd, argv); //handler exit_status
		if (execve(path, ast->expanded_cmd, env) == -1)
			exit(1); //must clean mm allocated
	}
	waitpid(pid, &exit_status, 0);
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

	if(!ft_strcmp(pipe_direction, "LEFT", 4))
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
	exit(exit_status); //must clean mm allocated
}

static char *get_path(char *command, char **env)
{
	char	*part_path;
	char	*possible_path;
	char	**paths;
	int		i;

	i = 0;
	if (ft_strchr(command, '/'))
		return (validate_access(cmd));
	while (ft_strncmp(s_pipex->env[i], "PATH=", 4))
		i++;
	paths = ft_split(s_pipex->env[i] + 5, ':');
	i = 0;
	while (paths[i])
	{
		part_path = ft_strjoin(paths[i++], "/");
		possible_path = ft_strjoin(part_path, cmd);
		if (!(access(possible_path, X_OK)))
		{
			clean_child_data(paths, NULL, part_path); //pipex
			return (possible_path);
		}
		clean_child_data(NULL, possible_path, part_path); //pipex
	}
	clean_matrix(paths); //pipex
	return (NULL);
}

static char	*validate_access(char *command)
{
	if (!(access(command, X_OK)))
		return (command);
	else
		return ("invalid");
}
