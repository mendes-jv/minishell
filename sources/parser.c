#include "../includes/minishell.h"

static t_ast	*parse_to_ast(t_dlist *words, t_parse_status *status, size_t precedence);
static t_ast	*command_to_ast(t_dlist **words, t_parse_status *status);
static bool		join_command(char **cmd, t_dlist **word);
static bool		append_redir(t_dlist **redirs, t_dlist **words, t_parse_status *status);
static void		set_parse_status(t_parse_status *status, enum e_parse_status new_status, t_dlist *word);
static void 	manage_error_status(t_parse_status status);
static void 	clear_node(t_ast **node);

void	parser(char *command_line, t_ast **ast)
{
	t_dlist			*words;
	t_parse_status	status;

	status.current = NO_ERROR;
	words = NULL;
	lexer(command_line, &words);
	if (!words)
		return ;
	*ast = parse_to_ast(words, &status, 0);
	if (status.current != NO_ERROR)
		manage_error_status(status);
	expand(ast);
	ft_dlstclear(&words, free, clear_token);
}

static t_ast	*parse_to_ast(t_dlist *words, t_parse_status *status, size_t precedence)
{
	t_ast	*left;
	t_ast	*right;
	t_ast 	*node;

	node = NULL;
	if (status->current != NO_ERROR || !words)
		return (NULL);
	if (is_binary_operator(words->content) || is_flag(words->content, R_PAR))
		return (set_parse_status(status, SYNTAX_ERROR, words), NULL);
	else if (is_flag(words->content, L_PAR))
	{
		words = words->next;
		left = parse_to_ast(words, status, 0);
		if (!left)
			return (set_parse_status(status, MEMORY_ERROR, words), NULL);
		if (!words || !is_flag(words->content, R_PAR))
			return (set_parse_status(status, SYNTAX_ERROR, words), left);
		words = words->next;
	}
	else
		return (command_to_ast(&words, status));
	while (is_binary_operator(words->content) && is_logical_operator(words->content) >= precedence)
	{
		words = words->next;
		if (!words)
			return (set_parse_status(status, SYNTAX_ERROR, words), left);
		right = parse_to_ast(words, status, is_logical_operator(words->content) + 1);
		if (!right)
			return (left);
		node = calloc(1, sizeof(t_ast)); //TODO: deal with memory alloc
		*node =  (t_ast) {
			((t_token *)words->content)->flag,
			NULL, NULL, NULL, left, right
		};
	}
	return (node);
}

static t_ast	*command_to_ast(t_dlist **words, t_parse_status *status)
{
	t_ast	*node;

	if (status->current != NO_ERROR)
		return (NULL);
	node = calloc(1, sizeof (t_ast));
	*node = (t_ast) {WORD, NULL, NULL, NULL, NULL, NULL};
	while (*words && (is_flag((*words)->content, WORD)
		|| is_redir((*words)->content)))
	{
		if (is_flag((*words)->content, WORD))
		{
			if (!join_command(&node->cmd, words))
				return (clear_node(&node), set_parse_status(status, MEMORY_ERROR, *words), NULL);
		}
		else if (!append_redir(&node->redirs, words, status))
			return (free(node->cmd), free(node), set_parse_status(status, MEMORY_ERROR, *words), NULL);
	}
	return (node);
}

static bool	join_command(char **cmd, t_dlist **word)
{
	char	*old_cmd;

	if (!*cmd)
		*cmd = ft_strdup("");
	if (!*cmd)
		return (false);
	while (*word && is_flag((*word)->content, WORD))
	{
		old_cmd = *cmd;
		*cmd = ft_strdjoin(*cmd, ((t_token *)(*word)->content)->value, WHITESPACE);
		free (old_cmd);
		if (!*cmd)
			return (false);
		*word = (*word)->next;
	}
	return (true);
}

static bool	append_redir(t_dlist **redirs, t_dlist **words, t_parse_status *status)
{
	void	*temp_redir;
	char	*dup_value;
	t_flag	flag;

	while (*words && is_redir((*words)->content))
	{
		flag = ((t_token *)(*words)->content)->flag;
		*words = (*words)->next;
		if (!*words || !is_flag((*words)->content, WORD))
			return (set_parse_status(status, SYNTAX_ERROR, *words), false);
		dup_value = ft_strdup(((t_token *)(*words)->content)->value);
		if (!dup_value)
			return (set_parse_status(status, MEMORY_ERROR, *words), false);
		temp_redir = &(t_redir) {dup_value, NULL,0, flag};
		ft_dlstadd_b(redirs, ft_dlstnew((temp_redir)));
		*words = (*words)->next;
	}
	return (true);
}

bool	is_binary_operator(t_token *token)
{
	return (is_flag(token, PIPE) || is_logical_operator(token));
}

bool	is_redir(t_token *token)
{
	return (is_flag(token, GREATER) || is_flag(token, LESSER)
		|| is_flag(token, D_GREATER) || is_flag(token, D_LESSER));
}

bool	is_logical_operator(t_token *token)
{
	return (is_flag(token, D_PIPE) || is_flag(token, D_AND));
}

bool	is_flag(t_token *token, t_flag flag)
{
	return (token->flag == flag);
}

static void	set_parse_status(t_parse_status *status, enum e_parse_status new_status, t_dlist *word)
{
	t_flag 	flag;

	if (!word)
		flag = END;
	else
		flag = ((t_token *) word->content)->flag;
	*status = (t_parse_status) {new_status, flag};
}

static void 	manage_error_status(t_parse_status status)
{
	t_word_pattern 	*patterns;

	patterns = (t_word_pattern[11]){
			{"|", PIPE}, {">", GREATER}, {"<", LESSER},
			{">>", D_GREATER}, {"<<", D_LESSER}, {"||", D_PIPE},
			{"&&", D_AND}, {"(", L_PAR}, {")", R_PAR}, {"newline", END}
	};
	if (status.current == SYNTAX_ERROR)
	{
		ft_putstr_fd(ANSI_COLOR_RED, STDERR_FILENO);
		ft_putstr_fd("zapshell: syntax error near unexpected token `", STDERR_FILENO);
		while (patterns->pattern && patterns->flag != status.flag)
			patterns++;
		ft_putstr_fd(patterns->pattern, STDERR_FILENO);
		ft_putendl_fd(ANSI_COLOR_RESET, STDERR_FILENO);
	}
	//TODO: this function originally set an exit status for minishell, clear ast and set to zero the status memory address
}

static void	clear_node(t_ast **node)
{
	if (!*node)
		return ;
	if ((*node)->cmd)
		free((*node)->cmd);
	if ((*node)->expanded_cmd)
		ft_for_each((void **) (*node)->expanded_cmd, free);
}

void	clear_token(void *token)
{
	if (!token)
		return ;
	if (((t_token *)token)->value)
		free(((t_token *)token)->value);
//	free(token);
}