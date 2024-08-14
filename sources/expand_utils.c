# include "../includes/minishell.h"

char	*clean_string(char *cmd);
char	*skip_single_quotes(char *cmd, size_t *index);
char	*skip_double_quotes(char *cmd, size_t *index);
char	*skip_dollar_sign(char *cmd, size_t *index);
char	*double_quotes_str(char *cmd, size_t *index);
char	*handle_str(char *cmd, size_t *index);
char	*handle_empty_cmd_strings(char *cmd);
char	**split_cmd(char *cmd);
char	**expand_string(char *cmd);
char	*ft_strfjoin(char *s1, char* s2);

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
	//Todo: check if this function is working as expected (not sure if it is correct)
}

char	**expand_string(char *cmd)
{
	char	**expanded_cmd;

	cmd = clean_string(cmd);
	if (!cmd)
		return (NULL);
	cmd = handle_empty_cmd_strings(cmd);
	if (!cmd)
		return (NULL);
	expanded_cmd = split_cmd(cmd);
	free(cmd);
	if (!expanded_cmd)
		return (NULL);
	//TODO: ft_glauber(expanded_cmd);
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
			clean = skip_single_quotes(clean, &index);
		else if (cmd[index] == '\"')
			clean = ft_strfjoin(clean, skip_double_quotes(cmd, &index));
		else if (cmd[index] == '$')
			clean = ft_strfjoin(clean, skip_dollar_sign(cmd, &index));
		else
			clean = ft_strfjoin(clean, handle_str(cmd, &index));
	}
	return (clean);
}

char	*handle_empty_cmd_strings(char *cmd)
{
	char	*tmp;
	char	*result;
	size_t	i;
	size_t	j;

	if ((cmd[0] == '\'' && cmd[1] == '\'' && !cmd[2]) || (cmd[0] == '"' && cmd[1] == '"' && !cmd[2]))
		return (cmd);
	tmp = ft_calloc(ft_strlen(cmd) + 1, sizeof(char));
	i = 0;
	j = 0;
	while (cmd[i])
	{
		if ((cmd[0] == '\'' && cmd[1] == '\'') || (cmd[0] == '"' && cmd[1] == '"'))
			i += 2;
		else
			tmp[j++] = cmd[i++];
	}
	result = ft_calloc(ft_strlen(cmd) + 1, sizeof(char));
	free(cmd);
	ft_strlcpy(result, tmp, ft_strlen(tmp) + 1);
	free(tmp);
	return (result);
}

// char	**split_cmd(char *cmd)
// {
//
// }

char	*skip_single_quotes(char *cmd, size_t *index) //TODO: check if necessary to insert  cmd[*index] as handle_str
{
	size_t	start;

	start = *index;
	(*index)++;
	while (cmd[*index] != '\'')
		(*index)++;
	(*index)++;
	return (ft_strfjoin(cmd, ft_substr(cmd, start, *index - start)));
}

char	*skip_double_quotes(char *cmd, size_t *index) //TODO: check if necessary to insert  cmd[*index] as handle_str
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

char	*skip_dollar_sign(char *cmd, size_t *index) //TODO: check if necessary to insert  cmd[*index] as handle_str
{
	size_t	start;
	char	*substring;
	// char	*env_var;

	(*index)++;
	if (ft_isdigit(cmd[*index]) || cmd[*index] == '@')
	{
		(*index)++;
		return (ft_strdup(""));
	}
	if (cmd[*index] == '?') {
		(*index)++;
		return (ft_itoa(0)); //TODO: return exit_status
	}
	if (!ft_isalnum(cmd[*index]) && cmd[*index] != '_')
		return (ft_strdup("$"));
	start = *index;
	while (ft_isalnum(cmd[*index]) || cmd[*index] == '_')
		(*index)++;
	substring = ft_substr(cmd, start, *index - start);
	return (substring);
	// env_var = get_env_var(env, substring); //TODO: check how to get env_list to compare here
	// if (!env_var)
	// 	return (free(substring), ft_strdup(""));
	// return (free(substring), ft_strdup(env_var));
}

char	*double_quotes_str(char *cmd, size_t *index) //TODO: check if necessary to insert  cmd[*index] as handle_str
{
	size_t	start;

	start = *index;
	while (cmd[*index] != '\"' && cmd[*index] != '$')
		(*index)++;
	return (ft_substr(cmd, start, *index - start));
}

char	*handle_str(char *cmd, size_t *index)
{
	size_t	start;

	start = *index;
	while (cmd[*index] && cmd[*index] != '\'' && cmd[*index] != '\"' && cmd[*index] != '$')
		(*index)++;
	return (ft_substr(cmd, start, *index - start));
}

char	*ft_strfjoin(char *s1, char *s2)
{
	char	*joined;
	size_t	joined_length;
	size_t i;
	size_t j;

	if (!s1 || !s2)
		return (NULL);
	joined_length = ft_strlen(s1) + ft_strlen(s2) + 1;
	joined = ft_calloc(joined_length, sizeof(char));
	if (!joined)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		joined[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
		joined[i++] = s2[j++];
	joined[i] = '\0';
	free(s1);
	free(s2);
	return (joined);
}
