#include "../includes/minishell.h"

static void	expand_redir(t_redir *redir, char **env);
static void	heredoc(char *value, pid_t *pipe_fds, char **env);
static bool	is_delimiter(char *doc_line, char *values);
static void	dlstiter_redir(t_dlist *lst, void (*f)(void *, char **), char **env);

void	expand(t_ast **ast, char **env)
{
	t_token flag;

	if (!*ast)
		return;
	flag.flag = (*ast)->flag;
	if (is_binary_operator(&flag))
	{
		expand(&(*ast)->left, env);
		//TODO: if !heredoc_sigint
		expand(&(*ast)->right, env);
	}
	else
	{
		if ((*ast)->cmd)
			(*ast)->expanded_cmd = expand_string((*ast)->cmd, env);
		dlstiter_redir((*ast)->redirs, (void (*)(void *, char **))expand_redir, env);
	}
}

char	**expand_string(char *cmd, char **env)
{
	// char	**expanded_cmd;

	cmd = clean_string(cmd, env);
	if (!cmd)
		return (NULL);
	cmd = handle_empty_cmd_strings(cmd);
	if (!cmd)
		return (NULL);
	return((char**)cmd);
	// expanded_cmd = split_cmd(cmd);
	// free(cmd);
	// if (!expanded_cmd)
	// 	return (NULL);
	// //TODO: ft_glauber(expanded_cmd);
	// return (expanded_cmd);
}

static void	expand_redir(t_redir *redir, char **env)
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
			heredoc(redir->value, pipe_fds, env);
		waitpid(pid, &pid, 0);
		//TODO:	signal(SIGQUIT, sigquit_handler);
		//TODO: child_sigint == true
		close(pipe_fds[1]);
		if (WIFEXITED(pid) && WEXITSTATUS(pid) == SIGINT)
			return ;
		redir->heredoc = pipe_fds[0];
	}
	else
		redir->expanded_values = expand_string(redir->value, env);
}

static void	dlstiter_redir(t_dlist *lst, void (*f)(void *, char **), char **env)
{
	t_dlist *temp_node;

	temp_node = lst;
	if (!lst || !f)
		return ;
	while (temp_node)
	{
		f(temp_node->content, env);
		temp_node = temp_node->next;
	}
}

static void	heredoc_sigint(__attribute__((unused)) pid_t sig)
{
	//TODO: ft_clean_ms();
	exit(SIGINT);
}

static void	heredoc(char *value, pid_t *pipe_fds, char **env)
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
			expand_heredoc(doc_line, pipe_fds[1], env);
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
