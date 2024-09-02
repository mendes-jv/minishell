/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear_minishell.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 14:09:34 by pmelo-ca          #+#    #+#             */
/*   Updated: 2024/08/20 17:35:56 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	clear_ast(t_ast *ast)
{
	if (!ast)
		return ;
	clear_ast(ast->left);
	clear_ast(ast->right);
	if (ast->cmd)
		free(ast->cmd);
	if (ast->expanded_cmd)
		clear_matrix(ast->expanded_cmd);
	ft_dlstclear(&ast->redirs, free, clear_redirs);
	free(ast);
}

void	clear_minishell(t_minishell *minishell)
{
	if(minishell->ast)
		clear_ast(minishell->ast);
	if(minishell->env_copy)
		clear_matrix(minishell->env_copy);
	if (minishell->command_line)
		free(minishell->command_line);
	if (minishell->path)
		free(minishell->path);
	if (minishell)
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

void	clear_token(void *token)
{
	if (!token)
		return ;
	if (((t_token *)token)->value)
		free(((t_token *)token)->value);
	free(token);
}

void	clear_ast_node(t_ast **node)
{
	if (!*node)
		return ;
	if ((*node)->cmd)
		free((*node)->cmd);
	if ((*node)->expanded_cmd)
		ft_for_each((void **)(*node)->expanded_cmd, free);
}
