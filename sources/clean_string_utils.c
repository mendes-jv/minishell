/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_string_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 17:38:14 by pmelo-ca          #+#    #+#             */
/*   Updated: 2024/09/11 19:54:05 by pmelo-ca         ###   ########.fr       */
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

char	*skip_double_quotes(char *cmd, size_t *index, char **env,
		int exit_status)
{
	char	*clean;

	clean = ft_strdup("\"");
	(*index)++;
	while (cmd[*index] && cmd[*index] != '"')
	{
		if (cmd[*index] == '$')
			clean = ft_strjoinf(clean, skip_dollar_sign(cmd, index, env,
						exit_status));
		else
			clean = ft_strjoinf(clean, double_quotes_str(cmd, index));
	}
	(*index)++;
	return (ft_strjoinf(clean, ft_strdup("\"")));
}

char	*skip_dollar_sign(char *cmd, size_t *index, char **env, int exit_status)
{
	size_t	start;
	char	*env_var;

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
	env_var = get_env_var(cmd, start, index, env);
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

int	get_biggest_number(int i, int j)
{
	if (i > j)
		return (i);
	return (j);
}
