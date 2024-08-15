#include "../includes/minishell.h"

char	*skip_single_quotes(char *cmd, size_t *index)
{
    size_t	start;

    start = *index;
    (*index)++;
    while (cmd[*index] && cmd[*index] != '\'')
        (*index)++;
    (*index)++;
    return (ft_strjoinf(cmd, ft_substr(cmd, start, *index - start)));
}

char	*skip_double_quotes(char *cmd, size_t *index, char **env)
{
    char	*clean;

    clean = ft_strdup("\"");
    (*index)++;
    while (cmd[*index] && cmd[*index] != '\"')
    {
        if (cmd[*index] == '$')
            clean = ft_strjoinf(clean, skip_dollar_sign(cmd, index, env));
        else
            clean = ft_strjoinf(clean, double_quotes_str(cmd, index));
    }
    (*index)++;
    return (ft_strjoinf(clean, ft_strdup("\"")));
}

char	*skip_dollar_sign(char *cmd, size_t *index, char **env)
{
    size_t	start;
    char	*substring;
    char	*env_var;
	int		i;

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
    if (!ft_isalnum(cmd[*index]) && cmd[*index] != '_') //TODO: check if need to include more chars to this validation
        return (ft_strdup("$"));
    start = *index;
    while (cmd[*index] && (ft_isalnum(cmd[*index]) || cmd[*index] == '_')) //TODO: check if need to include more chars to this validation
        (*index)++;
    substring = ft_substr(cmd, start, *index - start);
	i = 0;
	env_var = NULL;
	while (env[i])
	{
		if (ft_strncmp(substring, env[i], strlen_env(env[i])))
			env_var = ft_strdup(env[i]);
		i++;
	}
	free(substring);
	if (!env_var)
		return (ft_strdup(""));
	return (env_var);
}

char	*double_quotes_str(char *cmd, size_t *index)
{
    size_t	start;

    start = *index;
    while (cmd[*index] && cmd[*index] != '\"' && cmd[*index] != '$')
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
