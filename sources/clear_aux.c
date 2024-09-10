/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear_aux.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 17:20:11 by pmelo-ca          #+#    #+#             */
/*   Updated: 2024/08/20 17:22:02 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
