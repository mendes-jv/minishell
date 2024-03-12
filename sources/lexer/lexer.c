# include "../../includes/minishell.h"

t_status	lexer(char *command_line, t_list *words)
{
	char	*word;
	char	*start;
	char	*end;

	start = command_line;
	while (*start)
	{
		while (*start && ft_isspace(*start))
			start++;
		end = start;
		while (*end && !ft_isspace(*end))
			end++;
		if (end > start)
		{
			word = ft_substr(command_line, start - command_line, end - start);
			if (!word)
				return (FAILURE);
			if (ft_lstadd_back(&words, ft_lstnew(word)))
			{
				free(word);
				return (FAILURE);
			}
			start = end;
		}
	}
	return (SUCCESS);
}