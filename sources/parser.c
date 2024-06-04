#include "../includes/minishell.h"

static t_ast	*parse_to_ast(t_dlist *words, t_parse_status *status, size_t precedence);
static t_ast	*command_to_ast(t_dlist **words, t_parse_status *status);
static bool		join_command(char **cmd, t_dlist **word);
static bool		append_redir(t_dlist **redirs, t_dlist **word, t_parse_status *status);
static bool		is_binary_operator(t_token *token);
static bool		is_redir(t_token *token);
static bool		is_logical_operator(t_token *token);
static bool		is_flag(t_token *token, t_flag flag);
static void		set_parse_status(t_parse_status *status, t_parse_status new_status);
static void 	clear_node(t_ast **node);

void	parser(char *command_line, t_ast **ast)
{
	t_dlist			*words;
	t_parse_status	status;

	lexer(command_line, &words);
	if (!words)
		return ;
	while (words->next)
	{
		ft_printf("%s\n", ((t_token *)words->content)->value);
		words = words->next;
	}
	ft_printf("%s\n", ((t_token *)words->content)->value);
	while (words->prev)
		words = words->prev;
	//TODO: delete print loop after checking words is getting at it should
	status = NO_ERROR;
	*ast = parse_to_ast(words, &status, 0);
	if (status != NO_ERROR)
		; //TODO: handle error
	ft_dlstclear(&words, free, false);
}

static t_ast	*parse_to_ast(t_dlist *words, t_parse_status *status, size_t precedence)
{
	t_ast	*left;
	t_ast	*right;
	t_ast 	*node;

	if (*status != NO_ERROR || !words)
		return (NULL);
	if (is_binary_operator(words->content) || is_flag(words->content, L_PAR))
		return (set_parse_status(status, SYNTAX_ERROR), NULL);
	else if (is_flag(words->content, L_PAR))
	{
		words = words->next;
		left = parse_to_ast(words, status, 0);
		if (!left)
			return (set_parse_status(status, MEMORY_ERROR), NULL);
		if (!words || !is_flag(words->content, R_PAR))
			return (set_parse_status(status, SYNTAX_ERROR), left);
		words = words->next;
	}
	else
		return (command_to_ast(&words, status));
	while (is_binary_operator(words->content) && is_logical_operator(words->content) >= precedence)
	{
		words = words->next;
		if (!words)
			return (set_parse_status(status, SYNTAX_ERROR), left);
		right = parse_to_ast(words, status, is_logical_operator(words->content) + 1);
		if (!right)
			return (left);
		node =  &(t_ast) {
			((t_token *)words->content)->flag,
			NULL, NULL, NULL, left, right
		};
	}
	return (node);
}

static t_ast	*command_to_ast(t_dlist **words, t_parse_status *status)
{
	t_ast	*node;

	if (*status != NO_ERROR)
		return (NULL);
	node = &(t_ast) {WORD, NULL, NULL, NULL, NULL, NULL};
	while (*words && (is_flag((*words)->content, WORD)
		|| is_redir((*words)->content)))
	{
		if (is_flag((*words)->content, WORD))
		{
			if (!join_command(&node->cmd, words))
				return (clear_node(&node), set_parse_status(status, MEMORY_ERROR), NULL);
		}
		else if (!append_redir(&node->redirs, words, status))
			return (free(node->cmd), free(node), set_parse_status(status, MEMORY_ERROR), NULL);
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

static bool	append_redir(t_dlist **redirs, t_dlist **word, t_parse_status *status)
{
	void	*temp_redir;
	char	*dup_value;
	t_flag	flag;

	while (*word && is_redir((*word)->content))
	{
		flag = ((t_token *)(*word)->content)->flag;
		*word = (*word)->next;
		if (!*word || !is_flag((*word)->content, WORD))
			return (set_parse_status(status, SYNTAX_ERROR), false);
		dup_value = ft_strdup(((t_token *)(*word)->content)->value);
		if (!dup_value)
			return (set_parse_status(status, MEMORY_ERROR), false);
		temp_redir = &(t_redir) {dup_value, NULL,0, flag};
		ft_dlstadd_b(redirs, ft_dlstnew((temp_redir)));
		*word = (*word)->next;
	}
	return (true);
}

static bool	is_binary_operator(t_token *token)
{
	return (is_flag(token, PIPE) || is_logical_operator(token));
}

static bool	is_redir(t_token *token)
{
	return (is_flag(token, GREATER) || is_flag(token, LESSER)
		|| is_flag(token, D_GREATER) || is_flag(token, D_LESSER));
}

static bool	is_logical_operator(t_token *token)
{
	return (is_flag(token, D_PIPE) || is_flag(token, D_AND));
}
static bool	is_flag(t_token *token, t_flag flag)
{
	return (token->flag == flag);
}

static void	set_parse_status(t_parse_status *status, t_parse_status new_status)
{
	*status = new_status;
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
