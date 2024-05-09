#include "../includes/minishell.h"

//TODO: Set clean in fclean libft.

static void	*get_next_word(char **command_line);

bool	parser(char *command_line, t_ast *ast)
{
	t_list	*words;

	words = NULL;
	if (lexer(command_line, &words))
	{
		// ast->word = words->content;
		// ast->left = NULL;
		// ast->right = NULL;
		return (true);
	}
	return (false);
}

bool	lexer(char *command_line, t_list *words)
{
	while (*command_line)
	{
		ft_lstadd_back(
				&words,
				ft_lstnew(get_next_word(&command_line))
		);
	}
	return (true);
}

static void	*get_next_word(char **command_line)
{
	char	*value;
	t_flag	flag;
	size_t	word_len;

	word_len = 0;


	return ((void *) {value, flag});
}