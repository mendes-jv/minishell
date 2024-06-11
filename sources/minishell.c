#include "../includes/minishell.h"

void draw_ascii_art();

int main()
{
	char	*command_line;
	t_ast	*ast;

	draw_ascii_art();
	command_line = readline(PROMPT);
	while (command_line)
	{
		if (ft_strlen(command_line))
		{
			add_history(command_line);
			parser(command_line, &ast);
			execute(ast);
		}
		free(command_line);
		command_line = readline(PROMPT);
	}
	return (EXIT_SUCCESS);
}

void	draw_ascii_art()
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
