#include "../includes/minishell.h"

void	draw_ascii_art(void);

int	main(int argc, char **argv, char **envp)
{
	t_minishell	*minishell;

	((void)argc, (void)argv);
	draw_ascii_art();
	init_minishell(&minishell, envp);
	minishell->command_line = readline(PROMPT);
	while (minishell->command_line)
	{
		if (ft_strlen(minishell->command_line))
		{
			add_history(minishell->command_line);
			parser(&minishell); //TODO: losing WORDS and AST reference
			execute_ast(&minishell, false); // TODO: losing this reference; fix
			clear_ast(minishell->ast, minishell->words);
		}
		free(minishell->command_line);
		minishell->command_line = readline(PROMPT);
	}
	clear_minishell(minishell);
	return (minishell->exit_status);
}

void	init_minishell(t_minishell **minishell, char **envp)
{
	(*minishell) = calloc(1, sizeof(t_minishell));
	(**minishell) = (t_minishell){
		NULL, NULL, get_env_cpy(envp), NULL, NULL, 0, dup(0), dup(1)};
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
