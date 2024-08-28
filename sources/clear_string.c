/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear_string.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 17:38:14 by pmelo-ca          #+#    #+#             */
/*   Updated: 2024/08/20 17:38:15 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*skip_single_quotes(char *cmd, size_t *index)
{
	size_t	start;

	start = *index;
	(*index)++;
	while (cmd[*index] && cmd[*index] != '\'')
		(*index)++;
	(*index)++;
	return (ft_substr(cmd, start, *index - start));
}

char	*skip_double_quotes(char *cmd, size_t *index, char **env, int exit_status)
{
	char	*clean;

	clean = ft_strdup("\"");
	(*index)++;
	while (cmd[*index] && cmd[*index] != '"')
	{
		if (cmd[*index] == '$')
			clean = ft_strjoinf(clean, skip_dollar_sign(cmd, index, env, exit_status));
		else
			clean = ft_strjoinf(clean, double_quotes_str(cmd, index));
	}
	(*index)++;
	return (ft_strjoinf(clean, ft_strdup("\"")));
}

char	*skip_dollar_sign(char *cmd, size_t *index, char **env, int exit_status)
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
	if (cmd[*index] == '?')
	{
		(*index)++;
		if (WIFEXITED(exit_status))
			return (ft_itoa(WEXITSTATUS(exit_status)));
		return (ft_itoa(exit_status));
	}
	if (!ft_isalnum(cmd[*index]) && cmd[*index] != '_')
		return (ft_strdup("$"));
	start = *index;
	while (cmd[*index] && (ft_isalnum(cmd[*index]) || cmd[*index] == '_'))
		(*index)++;
	substring = ft_substr(cmd, start, *index - start);
	i = 0;
	env_var = NULL;
	while (env[i])
	{
		if (!ft_strncmp(substring, env[i], strlen_env(env[i])))
			env_var = ft_substr(env[i], ft_strlen(substring) + 1,
					ft_strlen(env[i]));
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
	while (cmd[*index] != '"' && cmd[*index] != '$')
		(*index)++;
	return (ft_substr(cmd, start, *index - start));
}

char	*handle_str(char *cmd, size_t *index)
{
	size_t	start;

	start = *index;
	while (cmd[*index] && cmd[*index] != '\'' && cmd[*index] != '\"'
		&& cmd[*index] != '$')
		(*index)++;
	return (ft_substr(cmd, start, *index - start));
}
