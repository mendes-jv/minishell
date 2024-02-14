#include "../includes/minishell.h"

void draw_ascii_art();

int main()
{
	char	*line;
	draw_ascii_art();
	line = readline("ZapShell> ");
	while (line)
	{
		line = readline("ZapShell> ");
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
