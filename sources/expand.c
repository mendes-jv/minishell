#include "../includes/minishell.h"

static void	expand_redir(t_redir *redir);
static void	heredoc(char *value, pid_t *pipe_fds);
static bool	is_delimiter(char *doc_line, char *values);

void	expand(t_ast **ast)
{
	t_token flag;

	if (!*ast)
		return;
	flag.flag = (*ast)->flag;
	if (is_binary_operator(&flag))
	{
		expand(&(*ast)->left);
		//TODO: if !heredoc_sigint
		expand(&(*ast)->right);
	}
	else
	{
		if ((*ast)->cmd)
			(*ast)->expanded_cmd = expand_string((*ast)->cmd);
		ft_dlstiter((*ast)->redirs, (void (*)(void *))expand_redir);
	}
}

static void	expand_redir(t_redir *redir)
{
	//TODO:	bool	child_sigint;
	pid_t 	pipe_fds[2];
	pid_t 	pid;

	if (redir->flag == D_LESSER)
	{
		pipe(pipe_fds);
		//TODO: child_sigint = false;
		signal(SIGQUIT, SIG_IGN);
		pid = fork();
		if (!pid)
			heredoc(redir->value, pipe_fds);
		waitpid(pid, &pid, 0);
		//TODO:	signal(SIGQUIT, sigquit_handler);
		//TODO: child_sigint == true
		close(pipe_fds[1]);
		if (WIFEXITED(pid) && WEXITSTATUS(pid) == SIGINT)
			return ;
		redir->heredoc = pipe_fds[0];
	}
	else
		redir->expanded_values = expand_string(redir->value);
}

static void	heredoc_sigint(__attribute__((unused)) pid_t sig)
{
	//TODO: ft_clean_ms();
	exit(SIGINT);
}

static void	heredoc(char *value, pid_t *pipe_fds)
{
	char	*doc_line;
	char	*quote_or_null;

	signal(SIGINT, heredoc_sigint);
	quote_or_null = value;
	while (*quote_or_null && !ft_strchr(QUOTES, *quote_or_null))
		quote_or_null++;
	doc_line = readline(HEREDOC_PROMPT);
	while (doc_line && !is_delimiter(doc_line, value))
	{
		if (!*quote_or_null)
			expand_heredoc(doc_line, pipe_fds[1]);
		else
			ft_putendl_fd(doc_line, pipe_fds[1]);
		free(doc_line);
		doc_line = readline(HEREDOC_PROMPT);
	}
	//TODO: ft_clean_ms();
	exit(EXIT_SUCCESS);
}

bool	is_delimiter(char *doc_line, char *value)
{
		while (*doc_line)
		{
			while (*value && ft_strchr(QUOTES, *value))
				value++;
			if (*doc_line == *value)
			{
				doc_line++;
				value++;
			}
			else
				return (false);
		}
		while (*value && ft_strchr(QUOTES, *value))
			value++;
		return (!*value);
}
