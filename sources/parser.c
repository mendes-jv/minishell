#include "../includes/minishell.h"

static bool		lexer(char *command_line, t_dlist *words);
static t_token	*get_next_word(char **command_line);
static char		*word_last_char(char *command_line);
static t_flag	get_word_type(char *word);

bool	parser(char *command_line, t_ast *ast)
{
	t_dlist	*words;

	if (lexer(command_line, words))
	{
		// ast->word = words->content;
		// ast->left = NULL;
		// ast->right = NULL;
	}
	ft_dlstclear(&words, free, false);
	return (true);
}

bool	lexer(char *command_line, t_dlist *words)
{
	while (*command_line)
	{
		ft_dlstadd_b(
				&words,
				ft_dlstnew(get_next_word(&command_line))
		);
	}
	return (true);
}

static t_token	*get_next_word(char **command_line)
{
	char	*word;
	char	*start;

	while (**command_line && **command_line == ' ')
		(*command_line)++;
	start = *command_line;
	*command_line = word_last_char(*command_line);
	if (!**command_line)
		(*command_line)--;
	word = ft_substr(start, 0, *command_line - start);
	return (&(t_token) {word, get_word_type(word)});
}

static char	*word_last_char(char *command_line)
{
	char quote_type;
	if (ft_strchr(METACHARS, *command_line))
	{
		if (ft_strchr(OPERATORS, *command_line) && *command_line == *(command_line + 1))
			return (command_line + 1);
		if (*command_line != '&')
			return (command_line);
	}
	if (ft_strchr(QUOTES, *command_line))
	{
		quote_type = *command_line;
		command_line++;
		while (*command_line && *command_line != quote_type)
			command_line++;
		return (command_line);
	}
	while (*command_line && *command_line != ' ' && !ft_strchr(METACHARS, *command_line))
			command_line++;
	return (command_line);
}

static t_flag	get_word_type(char *word)
{
	t_word_pattern *word_patterns;

	word_patterns = (t_word_pattern[11]){
		{"|", PIPE}, {">", GREATER}, {"<", LESSER},
		{">>", DOUBLE_GREATER}, {"<<", DOUBLE_LESSER}, {"||", DOUBLE_PIPE},
		{"&&", DOUBLE_AND}, {"(", LEFT_PARENTHESIS}, {")", RIGHT_PARENTHESIS},
		{"*", WILDCARD}, {NULL, WORD}
	};

	while (word_patterns->pattern)
	{
		if (!ft_strncmp(word, word_patterns->pattern, ft_strlen(word_patterns->pattern)))
			return (word_patterns->flag);
		word_patterns++;
	}
	return (WORD);
}
