#include "../includes/minishell.h"

static t_token	*get_next_token(char **command_line);
static char		*word_last_char(char *command_line);
static t_flag	get_word_type(char *word);

void	lexer(char *command_line, t_dlist **words)
{
	while (*command_line)
	{
		ft_dlstadd_b(
				words,
				ft_dlstnew(get_next_token(&command_line))
		);
	}
}

static t_token	*get_next_token(char **command_line)
{
	t_token *token;
	char	*word;
	char	*start;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	while (**command_line && ft_strchr(TAB_OR_SPACE, **command_line))
		(*command_line)++;
	start = *command_line;
	*command_line = word_last_char(*command_line);
	word = ft_substr(start, 0, *command_line - start);
	while (**command_line && ft_strchr(TAB_OR_SPACE, **command_line))
		(*command_line)++;
	*token = (t_token) {word,get_word_type(word)};
	return (token);
}

static char	*word_last_char(char *command_line)
{
	char quote_type;

	if (ft_strchr(METACHARS, *command_line))
	{
		if (ft_strchr(OPERATORS, *command_line) && *command_line == *(command_line + 1))
			return (command_line + 2);
		if (*command_line != '&')
			return (command_line + 1);
	}
	if (ft_strchr(QUOTES, *command_line))
	{
		quote_type = *command_line;
		command_line++;
		while (*command_line && *command_line != quote_type)
			command_line++;
		return (command_line + 1);
	}
	while (*command_line && !ft_strchr(TAB_OR_SPACE, *command_line) && !ft_strchr(METACHARS, *command_line))
		command_line++;
	return (command_line);
}

static t_flag	get_word_type(char *word)
{
	t_word_pattern *word_patterns;

	word_patterns = (t_word_pattern[11]){
			{"|", PIPE}, {">", GREATER}, {"<", LESSER},
			{">>", D_GREATER}, {"<<", D_LESSER}, {"||", D_PIPE},
			{"&&", D_AND}, {"(", L_PAR}, {")", R_PAR}, {NULL, WORD}
	};
	while (word_patterns->pattern)
	{
		if (!ft_strncmp(word, word_patterns->pattern, ft_strlen(word_patterns->pattern)))
			return (word_patterns->flag);
		word_patterns++;
	}
	return (WORD);
}
