#include "../includes/minishell.h"

void draw_ascii_art();

int main()
{
	char	*command_line;
	t_ast	ast;

	draw_ascii_art();
	command_line = readline("ZapShell> ");
	while (command_line)
	{
		if (ft_strlen(command_line))
		{
			add_history(command_line);
			if(parser(command_line, &ast))
				execute(&ast);
		}
		free(command_line);
		command_line = readline("ZapShell> ");
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
			ft_printf("\033[0;32m%c", *ascii_art++);
		else
			ft_printf("\033[0m%c", *ascii_art++);
	}
	ft_printf("\033[0m%s", ascii_art);
}
