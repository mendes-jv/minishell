/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 18:46:24 by pmelo-ca          #+#    #+#             */
/*   Updated: 2024/09/12 18:49:24 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	init_minishell(t_minishell **minishell, char **envp)
{
	(*minishell) = calloc(1, sizeof(t_minishell));
	(**minishell) = (t_minishell){
		NULL, NULL, NULL, get_env_cpy(envp), NULL, NULL, 0, dup(0), dup(1)};
	signals_non_interactive();
	(*minishell)->original_term = calloc(1, sizeof(struct termios));
	tcgetattr(STDIN_FILENO, (*minishell)->original_term);
}

void	draw_ascii_art(void)
{
	char	*ascii_art;

	ascii_art = ASCII_ART;
	while (*ascii_art != '_')
	{
		if (*ascii_art == '.')
			ft_printf(ASCII_GREEN, *ascii_art++);
		else
			ft_printf(ASCII_WHITE, *ascii_art++);
	}
	ft_printf(ASCII_RESET, ascii_art);
}

void	close_fds(int fd1, int fd2)
{
	close(fd1);
	close(fd2);
}
