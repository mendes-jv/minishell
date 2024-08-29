#include "../includes/minishell.h"

static t_token	*get_next_token(char **command_line);
static char		*word_last_char(char *command_line);
static t_flag	get_word_type(char *word);
static char		*quote_deal (char *command_line);

void	lexer(char *command_line, t_dlist **words)
{
	t_token *token;

	while (*command_line)
	{
		token = get_next_token(&command_line);
		if (!token)
		{
			ft_dlstclear(words, free, clear_token);
			break;
		}
		ft_dlstadd_b(words,	ft_dlstnew(token));
			//TODO: test behaviour with empty and only spaces command_line
	}
}

static t_token	*get_next_token(char **command_line)
{
	t_token	*token;
	char	*word;
	char	*start;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	while (**command_line && ft_strchr(TAB_OR_SPACE, **command_line))
		(*command_line)++;
	start = *command_line;
	*command_line = word_last_char(*command_line);
	if (!*command_line)
		return (NULL);
	word = ft_substr(start, 0, *command_line - start);
	while (**command_line && ft_strchr(TAB_OR_SPACE, **command_line))
		(*command_line)++;
	*token = (t_token){word, get_word_type(word)};
	return (token);
}

static char	*word_last_char(char *command_line)
{
	if (ft_strchr(METACHARS, *command_line))
	{
		if (ft_strchr(OPERATORS, *command_line)
			&& *command_line == *(command_line + 1))
			return (command_line + 2);
		if (*command_line != '&')
			return (command_line + 1);
	}
	if (ft_strchr(QUOTES, *command_line))
		return(quote_deal(command_line));
	while (*command_line && !ft_strchr(TAB_OR_SPACE, *command_line)
		&& !ft_strchr(METACHR_NO_AND, *command_line))
		if (*command_line == '&' && *(command_line + 1) == '&')
			return (command_line);
		else
			command_line++;
	return (command_line);
}

static t_flag	get_word_type(char *word)
{
	t_word_pattern	*word_patterns;
	size_t			word_len;

	word_patterns = (t_word_pattern[11]){
		{"|", PIPE}, {">", GREATER}, {"<", LESSER}, {">>", D_GREATER}, {"<<",
			D_LESSER}, {"||", D_PIPE}, {"&&", D_AND}, {"(", L_PAR}, {")",
			R_PAR}, {NULL, WORD}};
	while (word_patterns->pattern)
	{
		word_len = ft_strlen(word);
		if (word_len < 2)
			word_len = 2;
		if (!ft_strncmp(word, word_patterns->pattern, word_len))
			return (word_patterns->flag);
		word_patterns++;
	}
	return (WORD);
}

static char	*quote_deal(char *command_line) //TODO make this better
{
	char quote_type;
	int i;
	int	len;

	i = 0;
	quote_type = *command_line;
	len = ft_strlen(command_line);
	while (i < len)
	{
		command_line++;
		i++;
		if (quote_type == *command_line)
		{
			if (i < len)
			{
				command_line++;
				i++;
				if(quote_type != *command_line)
					return (command_line);
			}
		}
	}
	return(NULL);
}