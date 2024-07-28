# include "../includes/minishell.h"

void	expand_heredoc(char *doc_line, pid_t pipe_fd)
{
	char **expanded_doc_line;

	if (*doc_line == '$')
		if (*(doc_line + 1) == '?')
			(void) doc_line; //TODO: ft_putnbr_fd(exit_status, pipe_fd);
		else
		{
			expanded_doc_line = expand_string(doc_line); //TODO: check if expanded_doc_line can get NULL
			ft_putendl_fd(*expanded_doc_line, pipe_fd);
			free(*expanded_doc_line);
			free(expanded_doc_line);
		}
	else
		ft_putendl_fd(doc_line, pipe_fd);
}

char	**expand_string(char *cmd)
{
	char	**expanded_cmd;

	expanded_cmd = malloc(2 * sizeof(char *));
	expanded_cmd[0] = ft_strdup(cmd);
	return (expanded_cmd);
}
