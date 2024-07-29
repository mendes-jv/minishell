# include "../includes/minishell.h"

char	*clean_string(char *cmd);
char	*skip_single_quotes(char *cmd, size_t *index);
char	*skip_double_quotes(char *cmd, size_t *index);
char	*skip_dollar_sign(char *cmd, size_t *index);
char	**expand_string(char *cmd);

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

	cmd = clean_string(cmd);
	expanded_cmd = malloc(2 * sizeof(char *));
	expanded_cmd[0] = ft_strdup(cmd);
	return (expanded_cmd);
}

char	*clean_string(char *cmd)
{
	char	*clean;
	size_t	index;

	clean = ft_strdup("");
	index = 0;
	while (cmd[index])
	{
		if (cmd[index] == '\'')
			clean = skip_single_quotes(cmd, &index);
		else if (cmd[index] == '\"')
			clean = ft_strfjoin(cmd, skip_double_quotes(cmd, &index));
		else if (cmd[index] == '$')
			clean = ft_strfjoin(cmd, skip_dollar_sign(cmd, &index));
		else
			sdfgsdfgdf //TODO: join normal word
	}
	return (clean);
}

char	*skip_single_quotes(char *cmd, size_t *index)
{
	size_t	start;

	start = *index;
	(*index)++;
	while (cmd[*index] != '\'')
		(*index)++;
	(*index)++;
	return (ft_strfjoin(cmd, ft_substr(cmd, start, *index - start)));
}

char	*skip_double_quotes(char *cmd, size_t *index)
{
	char	*clean;

	clean = ft_strdup("\"");
	(*index)++;
	while (cmd[*index] != '\"')
	{
		if (cmd[*index] == '$')
			clean = ft_strfjoin(clean, skip_dollar_sign(cmd, index));
		else
			clean = ft_strfjoin(clean, double_quotes_str(cmd, index));
	}
	(*index)++;
	return (ft_strfjoin(clean, ft_strdup("\"")));
}

char	*skip_dollar_sign(char *cmd, size_t *index)
{
	size_t	start;
	char	*substring;
	char	*env_var;

	(*index)++;
	if (ft_isdigit(cmd[*index]) || cmd[*index] == '@')
	{
		(*index)++;
		return (ft_strdup(""));
	}
	else if (cmd[*index] == '?') {
		(*index)++;
		return (ft_itoa(0)); //TODO: return exit_status
	}
	else if (!ft_isalnum(cmd[*index]) && cmd[*index] != '_')
		return (ft_strdup("$"));
	start = *index;
	while (ft_isalnum(cmd[*index]) || cmd[*index] == '_')
		(*index)++;
	substring = ft_substr(cmd, start, *index - start);
	env_var = get_env_var(env, substring); //TODO: check how to get env_list to compare here
	if (!env_var)
		return (free(substring), ft_strdup(""));
	return (free(substring), ft_strdup(env_var));
}

char	*double_quotes_str(char *cmd, size_t *index)
{
	size_t	start;

	start = *index;
	while (cmd[*index] != '\"')
		(*index)++;
	return (ft_substr(cmd, start, *index - start));
}