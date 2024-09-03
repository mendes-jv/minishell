/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_flag_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 17:12:51 by pmelo-ca          #+#    #+#             */
/*   Updated: 2024/09/02 16:55:15 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	is_binary_operator(t_token *token)
{
	return (is_flag(token, PIPE) || !is_not_logical_operator(token));
}

bool	is_redir(t_token *token)
{
	return (is_flag(token, GREATER) || is_flag(token, LESSER) || is_flag(token,
			D_GREATER) || is_flag(token, D_LESSER));
}

bool	is_not_logical_operator(t_token *token)
{
	if (is_flag(token, D_PIPE) || is_flag(token, D_AND))
		return (false);
	return(true);
}

bool	is_flag(t_token *token, t_flag flag)
{
	return (token->flag == flag);
}
