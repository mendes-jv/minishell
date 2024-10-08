/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 17:13:07 by pmelo-ca          #+#    #+#             */
/*   Updated: 2024/09/16 11:42:49 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static t_ast	*parse_to_ast(t_dlist **words, t_parse_status *status,
					size_t precedence);
static t_ast	*command_to_ast(t_dlist **words, t_parse_status *status);
static t_ast	*create_ast_leaf(t_dlist **words, t_parse_status *status);
static t_ast	*ast_handle_binary_operators(t_dlist **words,
					t_parse_status *status, t_ast *left);

t_ast	*parser(t_minishell **minishell)
{
	t_parse_status	status;
	t_dlist			*temp_words;

	status.current = NO_ERROR;
	(*minishell)->words = NULL;
	lexer((*minishell)->command_line, &(*minishell)->words);
	if (!(*minishell)->words)
		set_parse_status(&status, INVALID_ERROR, NULL);
	if (status.current != NO_ERROR)
		return (manage_error_status(status, minishell), NULL);
	temp_words = (*minishell)->words;
	(*minishell)->ast = parse_to_ast(&(*minishell)->words, &status, 0);
	if ((*minishell)->words)
		set_parse_status(&status, SYNTAX_ERROR, (*minishell)->words);
	(*minishell)->words = temp_words;
	ft_dlstclear(&(*minishell)->words, free, clear_token);
	if (status.current != NO_ERROR)
		return (manage_error_status(status, minishell), NULL);
	expand(&(*minishell)->ast, minishell, &status);
	if (status.current != NO_ERROR)
		return (manage_error_status(status, minishell), NULL);
	return ((*minishell)->ast);
}

static t_ast	*parse_to_ast(t_dlist **words, t_parse_status *status,
		size_t precedence)
{
	t_ast	*left;

	if (status->current != NO_ERROR)
		manage_error_status(*status, NULL);
	left = create_ast_leaf(words, status);
	if (!left)
		return (NULL);
	while (*words && is_binary_operator((*words)->content)
		&& is_not_logical_operator((*words)->content) >= precedence)
		left = ast_handle_binary_operators(words, status, left);
	return (left);
}

static t_ast	*create_ast_leaf(t_dlist **words, t_parse_status *status)
{
	t_ast	*node;

	node = NULL;
	if (status->current != NO_ERROR || !words)
		return (NULL);
	if (is_binary_operator((*words)->content) || is_flag((*words)->content,
			R_PAR))
		return (set_parse_status(status, SYNTAX_ERROR, *words), NULL);
	if (is_flag((*words)->content, L_PAR))
	{
		if (!(*words)->next)
			return (set_parse_status(status, SYNTAX_ERROR, *words), NULL);
		*words = (*words)->next;
		node = parse_to_ast(words, status, 0);
		if (!node)
			return (set_parse_status(status, SYNTAX_ERROR, *words), NULL);
		if (!*words || !is_flag((*words)->content, R_PAR))
			return (set_parse_status(status, SYNTAX_ERROR, *words), node);
		*words = (*words)->next;
	}
	else
		node = command_to_ast(words, status);
	return (node);
}

static t_ast	*command_to_ast(t_dlist **words, t_parse_status *status)
{
	t_ast	*node;

	if (status->current != NO_ERROR)
		return (NULL);
	node = calloc(1, sizeof(t_ast));
	*node = (t_ast){WORD, NULL, NULL, NULL, NULL, NULL};
	while (*words && (is_flag((*words)->content, WORD)
			|| is_redir((*words)->content)))
	{
		if (is_flag((*words)->content, WORD))
		{
			if (!join_command(&node->cmd, words))
				return (clear_ast_node(&node), set_parse_status(status,
						MEMORY_ERROR, *words), NULL);
		}
		else if (!append_redir(&node->redirs, words, status))
			return (free(node->cmd), free(node), set_parse_status(status,
					SYNTAX_ERROR, *words), NULL);
	}
	return (node);
}

static t_ast	*ast_handle_binary_operators(t_dlist **words,
		t_parse_status *status, t_ast *left)
{
	t_ast	*right;
	t_ast	*node;
	t_token	*temp_flag;

	node = NULL;
	right = NULL;
	temp_flag = (t_token *)(*words)->content;
	*words = (*words)->next;
	if (!*words)
		return (set_parse_status(status, SYNTAX_ERROR, *words), left);
	right = parse_to_ast(words, status,
			is_not_logical_operator((*words)->content));
	if (!right)
		return (left);
	node = calloc(1, sizeof(t_ast));
	if (!node)
	{
		clear_ast_node(&right);
		clear_ast_node(&left);
		return (set_parse_status(status, MEMORY_ERROR, *words), NULL);
	}
	*node = (t_ast){
		temp_flag->flag, NULL, NULL, NULL, left, right};
	return (node);
}
