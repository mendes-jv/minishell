/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 17:11:25 by pmelo-ca          #+#    #+#             */
/*   Updated: 2024/09/11 19:58:39 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	exit_check(t_minishell **minishell, t_ast *node)
{
	if (node->expanded_cmd)
	{
		if (!ft_strncmp(node->expanded_cmd[0], "exit", 4))
			exec_exit(node->expanded_cmd, minishell);
	}
	(*minishell)->exit_status = 0;
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

bool	is_delimiter(char *doc_line, char *value)
{
	while (*doc_line)
	{
		while (*value && ft_strchr(QUOTES, *value))
			value++;
		if (*doc_line == *value)
		{
			doc_line++;
			value++;
		}
		else
			return (false);
	}
	while (*value && ft_strchr(QUOTES, *value))
		value++;
	return (!*value);
}

char	*quote_deal(char *command_line)
{
	char	quote_type;
	int		i;
	int		len;

	i = 0;
	quote_type = *command_line;
	len = ft_strlen(command_line);
	while (i < len)
	{
		command_line++;
		i++;
		if (quote_type == *command_line)
		{
			if (i < len)
			{
				command_line++;
				i++;
				if (quote_type != *command_line)
					return (command_line);
			}
		}
	}
	return (NULL);
}

char	*get_env_var(char *cmd, size_t start, size_t *index, char **env)
{
	char	*env_var;
	char	*substring;
	int		i;

	i = 0;
	env_var = NULL;
	substring = ft_substr(cmd, start, *index - start);
	while (env[i])
	{
		if (!ft_strncmp(substring, env[i],
				get_biggest_number(ft_strlen(substring), strlen_env(env[i]))))
			env_var = ft_substr(env[i], ft_strlen(substring) + 1,
					ft_strlen(env[i]));
		i++;
	}
	free(substring);
	return (env_var);
}

void	close_fds(int fd1, int fd2)
{
	close(fd1);
	close(fd2);
}