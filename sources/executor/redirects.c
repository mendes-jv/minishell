#include "../../includes/minishell.h"

int check_redirection(t_ast *ast);
static int redirect_in(t_redir *tmp_redirs);
static int redirect_out(t_redir *tmp_redirs);
static int redirect_append(t_redir *tmp_redirs);
static void redirect_heredoc(t_redir *tmp_redirs);

int check_redirection(t_ast *ast)
{
	int exit_status;
	t_dlist *tmp_redirs;

	tmp_redirs = ast->redirs;
	exit_status = 0;
	if (!tmp_redirs)
		return (exit_status);
	while (tmp_redirs)
	{
		if (tmp_redirs->content->flag == GREATER)
			exit_status = redirect_out(tmp_redirs->content);
		else if (tmp_redirs->content->flag == LESSER)
			exit_status = redirect_in(tmp_redirs->content);
		else if (tmp_redirs->content->flag == D_GREATER)
			exit_status = redirect_append(tmp_redirs->content);
		else if (tmp_redirs->content->flag == D_LESSER)
			redirect_heredoc(tmp_redirs->content);
		if (exit_status)
			return (exit_status);
		tmp_redirs = tmp_redirs->next;
	}
}

static int redirect_in(t_redir *tmp_redirs)
{
	int fd;

	if (!tmp_redirs->expanded_values || !tmp_redirs->expanded_values[1])
		return (1); // TODO ambiguous redirect (deal with error handler)
	fd = open(tmp_redirs->expanded_values[0], O_RDONLY);
	if (fd == -1)
	{
		if (errno == EACCES)
			error_handler(1, s_pipex, NULL, s_pipex->argv[1]); // TODO validate error handler
		error_handler(2, s_pipex, NULL, s_pipex->argv[1]); // TODO validate error handler
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

static int redirect_out(t_redir *tmp_redirs)
{
	int fd;

	if (!tmp_redirs->expanded_values || !tmp_redirs->expanded_values[1])
		return (1); // TODO ambiguous redirect (deal with error handler)
	fd = open(tmp_redirs->expanded_values[0], O_CREAT | O_WRONLY | O_TRUNC,
			  S_IRWXU | S_IRWXG | S_IRWXO));
	if (fd == -1)
		error_handler(1, s_pipex, NULL, s_pipex->argv[1]); // TODO validate error handler
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

static int redirect_append(t_redir *tmp_redirs)
{
	int fd;

	if (!tmp_redirs->expanded_values || !tmp_redirs->expanded_values[1])
		return (1); // TODO ambiguous redirect (deal with error handler)
	fd = open(tmp_redirs->expanded_values[0],O_CREAT | O_WRONLY | O_APPEND,
			  S_IRWXU | S_IRWXG | S_IRWXO));
	if (fd == -1)
		error_handler(1, s_pipex, NULL, s_pipex->argv[1]); // TODO validate error handler
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

static void redirect_heredoc(t_redir *tmp_redirs)
{
	dup2(tmp_redirs->heredoc, STDIN_FILENO);
	close(tmp_redirs->heredoc);
}