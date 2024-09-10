/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 13:28:20 by pmelo-ca          #+#    #+#             */
/*   Updated: 2024/09/10 16:16:40 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	signals_non_interactive(void)
{
	struct sigaction	act1;
	struct sigaction	act2;

	ft_memset(&act1, 0, sizeof(act1));
	act1.sa_flags = 0;
	act1.sa_handler = signals_new_line;
	sigaction(SIGINT, &act1, NULL);
	ft_memset(&act2, 0, sizeof(act2));
	act2.sa_flags = 0;
	act2.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &act2, NULL);
}

void	signals_interactive_parent(void)
{
	struct sigaction	act1;
	struct sigaction	act2;

	ft_memset(&act1, 0, sizeof(act1));
	act1.sa_flags = 0;
	act1.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &act1, NULL);
	ft_memset(&act2, 0, sizeof(act2));
	act2.sa_flags = 0;
	act2.sa_handler = signals_only_new_line;
	sigaction(SIGINT, &act2, NULL);
}

void	signals_interactive_child(void)
{
	struct sigaction	act1;

	ft_memset(&act1, 0, sizeof(act1));
	act1.sa_flags = 0;
	act1.sa_handler = SIG_DFL;
	sigaction(SIGQUIT, &act1, NULL);
}

void	signals_only_new_line(int signo)
{
	(void)signo;
	write(1, "\n", 1);
}

void	signals_new_line(int signo)
{
	(void)signo;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}
