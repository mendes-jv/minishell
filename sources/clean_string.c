#include "../includes/minishell.h"

char	*skip_single_quotes(char *cmd, size_t *index) //TODO: check if necessary to insert  cmd[*index] as handle_str
{
    size_t	start;

    start = *index;
    (*index)++;
    while (cmd[*index] != '\'')
        (*index)++;
    (*index)++;
    return (ft_strjoinf(cmd, ft_substr(cmd, start, *index - start)));
}

char	*skip_double_quotes(char *cmd, size_t *index) //TODO: check if necessary to insert  cmd[*index] as handle_str
{
    char	*clean;

    clean = ft_strdup("\"");
    (*index)++;
    while (cmd[*index] != '\"')
    {
        if (cmd[*index] == '$')
            clean = ft_strjoinf(clean, skip_dollar_sign(cmd, index));
        else
            clean = ft_strjoinf(clean, double_quotes_str(cmd, index));
    }
    (*index)++;
    return (ft_strjoinf(clean, ft_strdup("\"")));
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