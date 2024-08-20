/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 14:46:23 by pmelo-ca          #+#    #+#             */
/*   Updated: 2024/08/19 17:19:07 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	expand_heredoc(char *doc_line, pid_t pipe_fd, t_minishell **minishell)
{
	char	**expanded_doc_line;

	if (*doc_line == '$')
		if (*(doc_line + 1) == '?')
			(void)doc_line; //TODO: ft_putnbr_fd(exit_status, pipe_fd);
		else
		{
			expanded_doc_line = expand_string(doc_line, minishell);
			//TODO: check if expanded_doc_line can get NULL;
			ft_putendl_fd(*expanded_doc_line, pipe_fd);
			free(*expanded_doc_line);
			free(expanded_doc_line);
		}
	else
		ft_putendl_fd(doc_line, pipe_fd);
	//Todo: check if this function is working as expected (not sure if it is correct)
}

char	*clean_string(char *cmd, char **env)
{
	char	*clean;
	size_t	index;

	clean = ft_strdup("");
	index = 0;
	while (cmd[index])
	{
		if (cmd[index] == '\'')
			clean = skip_single_quotes(clean, &index);
		//TODO: fix function is not working
		else if (cmd[index] == '\"')
			clean = ft_strjoinf(clean, skip_double_quotes(cmd, &index, env));
		else if (cmd[index] == '$')
			clean = ft_strjoinf(clean, skip_dollar_sign(cmd, &index, env));
		else
			clean = ft_strjoinf(clean, handle_str(cmd, &index));
	}
	return (clean);
}

char	*handle_empty_cmd_strings(char *cmd)
{
	char	*tmp;
	char	*result;
	size_t	i;
	size_t	j;

	if ((cmd[0] == '\'' && cmd[1] == '\'' && !cmd[2]) || (cmd[0] == '"'
			&& cmd[1] == '"' && !cmd[2]))
		return (cmd);
	tmp = ft_calloc(ft_strlen(cmd) + 1, sizeof(char));
	i = 0;
	j = 0;
	while (cmd[i])
	{
		if ((cmd[0] == '\'' && cmd[1] == '\'') || (cmd[0] == '"'
				&& cmd[1] == '"'))
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

char	*ft_strjoinf(char *s1, char *s2)
{
	char	*joined;
	size_t	i;
	size_t	j;

	if (!s1 || !s2)
		return (NULL);
	joined = ft_calloc(ft_strlen(s1) + ft_strlen(s2) + 1, sizeof(char));
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
