/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:07:06 by pmelo-ca          #+#    #+#             */
/*   Updated: 2024/09/17 17:22:12 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	exit_handler_aux(char *message, char *command,
				t_minishell **minishell, int exit_status);

void	get_path(t_minishell **minishell, t_ast *node)
{
	char	**paths;
	int		i;

	i = 0;
	if (strchr(node->expanded_cmd[0], '/') || node->expanded_cmd[0][0] == '.')
		(*minishell)->path = ft_strdup(node->expanded_cmd[0]);
	while ((*minishell)->env_copy[i]
		&& ft_strncmp((*minishell)->env_copy[i], "PATH=", 4))
		i++;
	if (!(*minishell)->env_copy[i] || ft_strlen((*minishell)->env_copy[i]) <= 5)
		return ;
	paths = ft_split((*minishell)->env_copy[i] + 5, ':');
	check_path_exist(minishell, node, paths);
}

void	exit_handler(t_minishell **minishell, t_ast *node)
{
	if (!(*minishell)->path)
		exit_handler_aux(ERROR_EXEC_COM_NOT_FOUND, node->expanded_cmd[0],
			minishell, 127);
	if (access(node->expanded_cmd[0], F_OK) != 0)
		exit_handler_aux(ERROR_EXEC_INVALID_PATH, node->expanded_cmd[0],
			minishell, 127);
	else if (access(node->expanded_cmd[0], X_OK) == 0)
		exit_handler_aux(ERROR_EXEC_DIRECTORY, node->expanded_cmd[0], minishell,
			126);
	else
		exit_handler_aux(ERROR_EXEC_PERMISSION_DENY, node->expanded_cmd[0],
			minishell, 126);
}

void	reset_redirects(bool piped, t_minishell *minishell)
{
	if (piped)
		return ;
	dup2(minishell->stdin, 0);
	dup2(minishell->stdout, 1);
}

static void	exit_handler_aux(char *message, char *command,
		t_minishell **minishell, int exit_status)
{
	dprintf(2, message, command);
	clear_minishell(*minishell);
	exit(exit_status);
}
