/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 14:46:17 by pmelo-ca          #+#    #+#             */
/*   Updated: 2024/09/11 17:57:56 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	allocate_strings(char *cmd, char ***expanded_cmd, int word_counter);
static void	skip_words(char *cmd, int *i);
static void	fill_strings(char *cmd, char ***expanded_cmd);
static void	fill_strings_aux(char *cmd, char ***expanded_cmd, int *i, int j);

char	**split_cmd(char *cmd)
{
	char	**expanded_cmd;
	int		word_counter;
	int		i;

	i = 0;
	word_counter = 0;
	if (!cmd)
		return (NULL);
	while (cmd[i])
	{
		if (cmd[i] != ' ' && ++word_counter)
			skip_words(cmd, &i);
		while (cmd[i] && cmd[i] == ' ')
			i++;
	}
	expanded_cmd = ft_calloc(word_counter + 1, sizeof(char *));
	if (!expanded_cmd)
		return (NULL);
	if (!word_counter)
		return (free(expanded_cmd), NULL);
	allocate_strings(cmd, &expanded_cmd, word_counter);
	if (!expanded_cmd)
		return (NULL);
	fill_strings(cmd, &expanded_cmd);
	return (expanded_cmd);
}

static void	skip_words(char *cmd, int *i)
{
	char	quotes;

	while (cmd[*i] && cmd[*i] != ' ')
	{
		if (cmd[*i] != '\'' && cmd[*i] != '"')
			(*i)++;
		else
		{
			quotes = cmd[(*i)++];
			while (cmd[(*i)] != quotes)
				(*i)++;
			(*i)++;
		}
	}
}

static void	allocate_strings(char *cmd, char ***expanded_cmd, int word_counter)
{
	int	start;
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (cmd[i])
	{
		if (cmd[i] != ' ')
		{
			start = i;
			skip_words(cmd, &i);
			(*expanded_cmd)[j] = ft_calloc(i - start + 1, sizeof(char));
			if (!(*expanded_cmd)[j])
			{
				clear_matrix(*expanded_cmd);
				*expanded_cmd = NULL;
				return ;
			}
			j++;
		}
		while (cmd[i] && cmd[i] == ' ')
			i++;
	}
	(*expanded_cmd)[word_counter] = NULL;
}

static void	fill_strings(char *cmd, char ***expanded_cmd)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (*expanded_cmd)
	{
		while (cmd[i] && (*expanded_cmd)[j])
		{
			if (cmd[i] != ' ')
			{
				fill_strings_aux(cmd, expanded_cmd, &i, j);
				j++;
			}
			while (cmd[i] && cmd[i] == ' ')
				i++;
		}
	}
}

static void	fill_strings_aux(char *cmd, char ***expanded_cmd, int *i, int j)
{
	char	quotes;
	int		k;

	k = 0;
	while (cmd[(*i)] && cmd[(*i)] != ' ')
	{
		if (cmd[(*i)] != '\'' && cmd[(*i)] != '"')
			(*expanded_cmd)[j][k++] = cmd[(*i)++];
		else
		{
			quotes = cmd[(*i)++];
			(*expanded_cmd)[j][k++] = quotes;
			while (cmd[(*i)] && cmd[(*i)] != quotes)
				(*expanded_cmd)[j][k++] = cmd[(*i)++];
			(*expanded_cmd)[j][k++] = cmd[(*i)++];
		}
	}
}
