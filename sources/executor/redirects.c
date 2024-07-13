#include "../../includes/minishell.h"

static int redirect_in(t_redir *tmp_redirs);
static int redirect_out(t_redir *tmp_redirs);
static int redirect_append(t_redir *tmp_redirs);
static void redirect_heredoc(t_redir *tmp_redirs);

int check_redirection(t_ast *ast)
{
	int exit_status;
	t_dlist *dlist_redirs;
	t_redir *temp_redir;

	dlist_redirs = ast->redirs;
	exit_status = 0;
	if (!dlist_redirs)
		return (exit_status);
	while (dlist_redirs)
	{
		temp_redir = (t_redir *)dlist_redirs->content;
		if (temp_redir->flag == GREATER)
			exit_status = redirect_out(temp_redir);
		else if (temp_redir->flag == LESSER)
			exit_status = redirect_in(temp_redir);
		else if (temp_redir->flag == D_GREATER)
			exit_status = redirect_append(temp_redir);
		else if (temp_redir->flag == D_LESSER)
			redirect_heredoc(temp_redir);
		if (exit_status)
			return (exit_status);
		dlist_redirs = dlist_redirs->next;
	}
	return(exit_status);
}

static int redirect_in(t_redir *tmp_redirs)
{
	int fd;

	if (!tmp_redirs->expanded_values || !tmp_redirs->expanded_values[1])
		return (exec_error_handler(1, "ambiguous redirect\n", tmp_redirs->value));
	fd = open(tmp_redirs->expanded_values[0], O_RDONLY);
	if (fd == -1)
	{
		if (errno == EACCES)
			return(exec_error_handler(1, "No such file or directory\n", tmp_redirs->value));
		return(exec_error_handler(1, "Permission denied\n", tmp_redirs->value));
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

static int redirect_out(t_redir *tmp_redirs)
{
	int fd;

	if (!tmp_redirs->expanded_values || !tmp_redirs->expanded_values[1])
		return(exec_error_handler(1, "ambiguous redirect\n", tmp_redirs->value));
	fd = open(tmp_redirs->expanded_values[0], O_CREAT | O_WRONLY | O_TRUNC,
			  S_IRWXU | S_IRWXG | S_IRWXO);
	if (fd == -1)
		return(exec_error_handler(1, "Permission denied\n", tmp_redirs->value));
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

static int redirect_append(t_redir *tmp_redirs)
{
	int fd;

	if (!tmp_redirs->expanded_values || !tmp_redirs->expanded_values[1])
		return(exec_error_handler(1, "ambiguous redirect\n", tmp_redirs->value));
	fd = open(tmp_redirs->expanded_values[0],O_CREAT | O_WRONLY | O_APPEND,
			  S_IRWXU | S_IRWXG | S_IRWXO);
	if (fd == -1)
		return(exec_error_handler(1, "Permission denied\n", tmp_redirs->value));
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

static void redirect_heredoc(t_redir *tmp_redirs)
{
	dup2(tmp_redirs->heredoc, STDIN_FILENO);
	close(tmp_redirs->heredoc);
}