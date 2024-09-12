/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 19:13:40 by pmelo-ca          #+#    #+#             */
/*   Updated: 2024/09/12 19:15:45 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	doc_line_null_aux(void);

int	heredoc_aux(char *value, char *quote_or_null, pid_t fd,
		t_minishell **minishell)
{
	int		status;
	char	*doc_line;

	status = 0;
	while (true)
	{
		doc_line = readline(HEREDOC_PROMPT);
		if (doc_line)
		{
			if (is_delimiter(doc_line, value))
				break ;
			if (!*quote_or_null)
				expand_heredoc(doc_line, fd, minishell);
			else
				ft_putendl_fd(doc_line, fd);
			free(doc_line);
		}
		else
		{
			status = doc_line_null_aux();
			break ;
		}
	}
	return (status);
}

static int	doc_line_null_aux(void)
{
	int	status;

	status = 0;
	if (g_signal == 0)
		printf(HEREDOC_WARNING);
	else
		status = SIGINT;
	return (status);
}
