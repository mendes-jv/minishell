/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 12:42:46 by pmelo-ca          #+#    #+#             */
/*   Updated: 2024/09/10 17:16:26 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	signals_heredoc_child(void)
{
	struct sigaction	act;

	ft_memset(&act, 0, sizeof(act));
	act.sa_flags = 0;
	act.sa_handler = SIG_DFL;
	sigaction(SIGINT, &act, NULL);
}

void	signals_heredoc_parent(void)
{
	struct sigaction	act;

	ft_memset(&act, 0, sizeof(act));
	act.sa_flags = 0;
	act.sa_handler = signals_only_new_line;
	sigaction(SIGINT, &act, NULL);
}
