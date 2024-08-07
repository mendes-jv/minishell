#include "../includes/minishell.h"

void	draw_ascii_art(void);

int	main(void)
{
	t_minishell	*minishell;

	draw_ascii_art();
	init_minishell(&minishell, envp);
	while (minishell->command_line)
	{
		if (ft_strlen(minishell->command_line))
		{
			add_history(minishell->command_line);
			parser(minishell->command_line, &minishell->ast);
			execute_ast(&minishell, false);
			clear_ast(minishell->ast);
		}
		free(minishell->command_line);
		minishell->command_line = readline(PROMPT);
	}
	clear_minishell(minishell);
	return (minishell->exit_status);
}

void	init_minishell(t_minishell **minishell, char **envp)
{
	(*minishell) = &(t_minishell){
		NULL, get_env_cpy(envp), readline(PROMPT), NULL, 0};
}

void	draw_ascii_art(void)
{
	char	*ascii_art;

	ascii_art = ASCII_ART;
	while (*ascii_art != '_')
	{
		if (*ascii_art == '/')
			ft_printf(ASCII_GREEN, *ascii_art++);
		else
			ft_printf(ASCII_WHITE, *ascii_art++);
	}
	ft_printf(ASCII_RESET, ascii_art);
}
