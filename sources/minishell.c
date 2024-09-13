/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 17:05:04 by pmelo-ca          #+#    #+#             */
/*   Updated: 2024/09/13 16:58:49 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

volatile int	g_signal = 0;

int	main(int argc, char **argv, char **envp)
{
	t_minishell	*minishell;
	t_ast		*parser_exec;
	int			status;

	((void)argc, (void)argv);
	draw_ascii_art();
	init_minishell(&minishell, envp);
	minishell->command_line = readline(PROMPT);
	while (minishell->command_line)
	{
		if (ft_strlen(minishell->command_line))
		{
			add_history(minishell->command_line);
			parser_exec = parser(&minishell);
			if (parser_exec)
				execute_ast(&minishell, false, parser_exec);
			clear_ast(minishell->ast);
		}
		free(minishell->command_line);
		minishell->command_line = readline(PROMPT);
	}
	status = minishell->exit_status;
	clear_minishell_eof(minishell);
	printf("exit\n");
	return (status);
}
