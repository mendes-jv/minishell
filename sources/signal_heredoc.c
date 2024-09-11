/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 12:42:46 by pmelo-ca          #+#    #+#             */
/*   Updated: 2024/09/11 17:35:52 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	signals_global(int signo);

void	signals_heredoc_child(void)
{
	struct sigaction	act;

	ft_memset(&act, 0, sizeof(act));
	act.sa_handler = signals_global;
	sigaction(SIGINT, &act, NULL);
}

void	signals_heredoc_parent(void)
{
	struct sigaction	act;

	ft_memset(&act, 0, sizeof(act));
	act.sa_handler = signals_only_new_line;
	sigaction(SIGINT, &act, NULL);
}

static void	signals_global(int signo)
{
	(void)signo;
	g_signal = 1;
}
