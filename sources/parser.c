/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 17:13:07 by pmelo-ca          #+#    #+#             */
/*   Updated: 2024/08/20 17:18:45 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
static t_ast	*parse_to_ast(t_dlist **words, t_parse_status *status,
					size_t precedence);
static t_ast	*command_to_ast(t_dlist **words, t_parse_status *status);
//static void 	print_matrix(char **str);
//static void 	print_ast(t_minishell **minishell);
static t_ast 	*create_ast_leaf(t_dlist **words, t_parse_status *status);

void	parser(t_minishell **minishell)
{
	t_parse_status	status;

	status.current = NO_ERROR;
	(*minishell)->words  = NULL;
	lexer((*minishell)->command_line, &(*minishell)->words);
	if (!(*minishell)->words)
		return ;
	(*minishell)->ast = parse_to_ast(&(*minishell)->words, &status, 0);
	if (status.current != NO_ERROR)
		manage_error_status(status);
	expand(&(*minishell)->ast, minishell);
//	print_ast(minishell);
}

static t_ast	*parse_to_ast(t_dlist **words, t_parse_status *status,
		size_t precedence)
{
	t_ast	*left;
	t_ast	*right;
	t_ast 	*node;

	if (status->current != NO_ERROR)
		manage_error_status(*status);
	left = create_ast_leaf(words, status);
	if (!*words)
		return (left);
	while (is_binary_operator((*words)->content) && is_logical_operator((*words)->content) >= precedence)
	{
		*words = (*words)->next;
		if (!*words)
			return (set_parse_status(status, SYNTAX_ERROR, *words), left);
		right = parse_to_ast(words, status, is_logical_operator((*words)->content) + 1);
		if (!right)
			return (left);
		node = calloc(1, sizeof(t_ast));
		if (!node)
		{
			clear_ast_node(&right);
			clear_ast_node(&left);
			return (set_parse_status(status, MEMORY_ERROR, *words), NULL);
		}
		*node = (t_ast) {
			1, NULL, NULL, NULL, left, right}; //TODO: fix flag
		if (!*words)
			break;
	}
	return (node);
}

static t_ast 	*create_ast_leaf(t_dlist **words, t_parse_status *status)
{
	t_ast	*node;

	node = NULL;
	if (status->current != NO_ERROR || !words)
		return (NULL);
	if (is_binary_operator((*words)->content) || is_flag((*words)->content, R_PAR))
		return (set_parse_status(status, SYNTAX_ERROR, *words), NULL);
	else if (is_flag((*words)->content, L_PAR))
	{
		*words = (*words)->next;
		node = create_ast_leaf(words, status);
		if (!node)
			return (set_parse_status(status, MEMORY_ERROR, *words), NULL);
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
					MEMORY_ERROR, *words), NULL);
	}
	return (node);
}

//static void print_ast(t_minishell **minishell)
//{
//	if (!(*minishell)->ast)
//		return;
//	printf("EXPANDED COMMAND: ");
//	print_matrix((*minishell)->ast->expanded_cmd);
//	printf("\nFLAG: %d\n", (*minishell)->ast->flag);
//	if ((*minishell)->ast->left) {
//		(*minishell)->ast = (*minishell)->ast->left;
//		print_ast(minishell);
//	}
//	if ((*minishell)->ast->right) {
//		(*minishell)->ast = (*minishell)->ast->right;
//		print_ast(minishell);
//	}
//}
//
//static void print_matrix(char **str)
//{
//	int i = 0;
//
//	if (!*str)
//		return;
//	while (str[i] != NULL)
//	{
//		printf(" %s " ,str[i]);
//		i++;
//	}
//}
//
