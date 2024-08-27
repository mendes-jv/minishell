/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strip_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 17:03:47 by pmelo-ca          #+#    #+#             */
/*   Updated: 2024/08/27 17:05:03 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	unquoted_len(char *str);
static void	fill_unquoted_string(const char *expanded_str, int *j,
				char *quoted_str, int *k);

static int	unquoted_len(char *str)
{
	size_t	i;
	size_t	len;
	char	quotes;

	i = 0;
	len = 0;
	quotes = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			if (!quotes)
				quotes = str[i++];
			else if (str[i] == quotes)
				quotes = ((i++) && 0);
			else
				len += (i++ || 1);
		}
		else
			len += (i++ || 1);
	}
	return (len);
}

static void	fill_unquoted_string(const char *expanded_str, int *i,
		char *quoted_str, int *j)
{
	char	quotes;

	quotes = expanded_str[(*i)++];
	while (expanded_str[*i] != quotes)
		quoted_str[(*j)++] = expanded_str[(*i)++];
	(*i)++;
}

char	*strip_quotes(char *expanded_cmd)
{
	char	*unquoted_str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	unquoted_str = ft_calloc(1 + unquoted_len(expanded_cmd), sizeof(char));
	if (!unquoted_str)
		return (NULL);
	while (expanded_cmd[i])
	{
		if (expanded_cmd[i] == '"' || expanded_cmd[i] == '\'')
			(fill_unquoted_string(expanded_cmd, &i, unquoted_str, &j));
		else
			unquoted_str[j++] = expanded_cmd[i++];
	}
	free(expanded_cmd);
	return (unquoted_str);
}
