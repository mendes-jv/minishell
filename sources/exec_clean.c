/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_clean.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 14:09:34 by pmelo-ca          #+#    #+#             */
/*   Updated: 2024/08/19 17:17:45 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	clear_ast(t_ast *ast, t_dlist *words)
{
	if (!ast)
		return ;
	clear_ast(ast->left, words);
	clear_ast(ast->right, words);
	if (ast->cmd)
		free(ast->cmd);
	if (ast->expanded_cmd)
		clear_matrix(ast->expanded_cmd);
	ft_dlstclear(&ast->redirs, free, clear_redirs);
	free(ast);
	if (words)
	{
		ft_dlstclear(&words, free, clear_token);
		free(words);
	}
}

void	clear_minishell(t_minishell *minishell)
{
	clear_ast(minishell->ast, minishell->words);
	clear_matrix(minishell->env_copy);
	if (minishell->command_line)
		free(minishell->command_line);
	if (minishell->path)
		free(minishell->path);
	free(minishell);
	rl_clear_history();
}

void	clear_redirs(void *redirs)
{
	if (!redirs)
		return ;
	if (((t_redir *)redirs)->value)
		free(((t_redir *)redirs)->value);
	if (((t_redir *)redirs)->expanded_values)
		clear_matrix(((t_redir *)redirs)->expanded_values);
	free(redirs);
}

void	clear_matrix(char **matrix)
{
	int	x;

	x = 0;
	if (!matrix)
		return ;
	while (matrix[x])
		free(matrix[x++]);
	free(matrix);
}

void	clean_child_data(char **matrix, char *possible_path, char *part_path)
{
	if (matrix)
		clear_matrix(matrix);
	if (possible_path)
		free(possible_path);
	if (part_path)
		free(part_path);
}
