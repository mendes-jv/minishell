#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libraries/libft/includes/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <fcntl.h>

# ifndef ASCII_ART
#  define ASCII_ART "\
                             @@@@@@@@@@@@@@@@@@@@@@@\n\
                        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n\
                     @@@@@@@@@@///////////////////@@@@@@@@@@\n\
                  @@@@@@@@/////////////////////////////@@@@@@@@\n\
                @@@@@@@////////////////////////////////////@@@@@@\n\
               @@@@@@///////////////////////////////////////@@@@@@\n\
             @@@@@@/////////@@@@@/////////////////////////////@@@@@\n\
            @@@@@@////////@@@@@@@@/////////////////////////////@@@@@\n\
            @@@@@////////@@@@@@@@@@/////////////////////////////@@@@@\n\
           @@@@@/////////@@@@@@@@@@/////////////////////////////(@@@@@\n\
           @@@@@//////////@@@@@@@////////////////////////////////@@@@@\n\
           @@@@@////////////@@@@@@///////////////////////////////@@@@@\n\
           @@@@@/////////////@@@@@@@/////////////////////////////@@@@@\n\
           @@@@@///////////////@@@@@@@@////////@@@@//////////////@@@@@\n\
            @@@@@/////////////////@@@@@@@@@@/@@@@@@@@@@/////////@@@@@@\n\
             @@@@@///////////////////@@@@@@@@@@@@@@@@@@////////@@@@@@\n\
              @@@@@///////////////////////@@@@@@@@@@@/////////@@@@@@\n\
               @@@@@////////////////////////////////////////@@@@@@\n\
              @@@@@///////////////////////////////////////@@@@@@@\n\
             @@@@@//////////////////////////////////////@@@@@@@@\n\
             @@@@////@@@@@@@//////////////////////@@@@@@@@@\n\
            @@@@@@@@@@@@@@@@@@@@@@@@@@@////@@@@@@@@@@@@@@\n\
           @@@@@@@           @@@@@@@@@@@@@@@@@@@@@@@\n\n\
      __________                   _________.__             .__   .__   \n\
      \\____    /_____   ______    /   _____/|  |__    ____  |  |  |  |  \n\
        /     / \\__  \\  \\____ \\   \\_____  \\ |  |  \\ _/ __ \\ |  |  |  |  \n\
       /     /_  / __ \\_|  |_> >  /        \\|   Y  \\\\  ___/ |  |__|  |__\n\
      /_______ \\(____  /|   __/  /_______  /|___|  / \\___  >|____/|____/\n\
              \\/     \\/ |__|             \\/      \\/      \\/\n\n"
# endif

# define ERROR_PWD_ARGS "zapshell: pwd: -%s: invalid option\n"
# define ERROR_UNSET_ID "zapshell: unset: '%s': not a valid identifier\n"
# define ERROR_EXPORT_ID "zapshell: export: '%s': not a valid identifier\n"
# define ERROR_ENV_OPTION "env: invalid option -- '%c'\n"
# define ERROR_ENV_MANY_ARGS "env: '%s': No such file or directory\n"
# define ERROR_CD_MANY_ARGS "zapshell: cd: too many arguments\n"
# define ERROR_CD_INVALID_PATH "zapshell: cd: %s: Invalid file or directory\n"
# define ERROR_EXIT_MANY_ARGS "exit\nzapshell: exit: too many arguments\n"
# define ERROR_EXIT_INVALID_ARG "exit\nzapshell: exit: %s: numeric argument required\n"
# define ERROR_AMBIGUOUS_REDIRECT "zapshell: %s: ambiguous redirect\n"
# define ERROR_EXEC_INVALID_PATH "zapshell: %s: No such file or directory\n"
# define ERROR_EXEC_PERMISSION_DENY "zapshell: %s: Permission denied\n"
# define ERROR_EXEC_COM_NOT_FOUND "zapshell: %s: command not found\n"
# define ERROR_EXEC_INVALID_PATH "zapshell: %s: No such file or directory\n"
# define ASCII_GREEN "\033[0;32m%c"
# define ASCII_WHITE "\033[0m%c"
# define ASCII_RESET "\033[0m%s"
# define PROMPT "ZapShell -> "
# define HEREDOC_PROMPT "ZapDoc> "
# define METACHARS "><&|()"
# define METACHR_NO_AND "><()|"
# define OPERATORS "><&|"
# define QUOTES "'\""
# define WHITESPACE " "
# define TAB_OR_SPACE " \t"

typedef enum e_flag
{
	WORD,
	PIPE,
	GREATER,
	LESSER,
	D_GREATER,
	D_LESSER,
	D_PIPE,
	D_AND,
	L_PAR,
	R_PAR,
	END,
}	t_flag;

typedef struct s_token
{
	char	*value;
	t_flag	flag;
}	t_token;

typedef struct s_word_pattern
{
	char	*pattern;
	t_flag	flag;
}	t_word_pattern;

enum e_parse_status
{
	MEMORY_ERROR,
	SYNTAX_ERROR,
	NO_ERROR
} ;

typedef struct s_parse_status
{
	enum e_parse_status	current;
	t_flag				flag;
}	t_parse_status;

typedef struct s_redir
{
	char	*value;
	char 	**expanded_values;
	int 	heredoc;
	t_flag	flag;
}	t_redir;

typedef struct s_ast
{
	t_flag			flag;
	t_dlist			*redirs;
	char 			*cmd;
	char			**expanded_cmd;
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;

typedef struct s_minishell
{
	t_ast	*ast;
	char	**env_copy;
	char	*command_line;
	char	*path;
	int		exit_status;
}	t_minishell;

void 	lexer(char *command_line, t_dlist **words);
void	parser(char *command_line, t_ast **ast, char **env);
void	expand(t_ast **ast, char **env);
char	**split_cmd(char *cmd);
char	*clean_string(char *cmd, char **env);
char	*ft_strjoinf(char *s1, char* s2);
char	*skip_single_quotes(char *cmd, size_t *index);
char	*skip_double_quotes(char *cmd, size_t *index);
char	*skip_dollar_sign(char *cmd, size_t *index);
char	*double_quotes_str(char *cmd, size_t *index);
char	*handle_str(char *cmd, size_t *index);
char	**expand_string(char *string, char **env);
void 	expand_heredoc(char *doc_line, pid_t pipe_fd);
char	*handle_empty_cmd_strings(char *cmd);
void	execute(t_ast *ast);
bool	is_binary_operator(t_token *token);
bool	is_redir(t_token *token);
bool	is_logical_operator(t_token *token);
bool	is_flag(t_token *token, t_flag flag);
void 	init_minishell(t_minishell **minishell, char **envp);

void	execute_ast(t_minishell **minishell, bool piped);
void	get_path(t_minishell **minishell);
void 	check_redirection(t_minishell **minishell);
void 	reset_redirects(bool piped);

bool	is_builtin(char *command);
int 	builtin_exec(t_minishell **minishell);
int		exec_echo(char **command);
int		exec_pwd(char **command);
int		exec_export(char **command, char ***env);
int		exec_unset(char **command, char ***env);
int		exec_env(char **command, char **env);
void    exec_exit(char **command);
int	    exec_cd(char **command, char ***env);

char	*lltoa(long long c);
int     get_array_len(char **arr);
char	**get_env_cpy(char **envp);
int     strlen_env(char *command);
bool	isvalid_num(char *command);
bool	isnumber(char *string);
int		error_handler(int exit_status, int fd, char *message, char *command);
void	exit_handler(char *message, char *command, t_minishell **minishell, bool print, int exit_status);

//Clear functions
void		clear_token(void *token);
void	    clear_matrix(char **matrix);
void        clear_ast(t_ast *ast);
void		clear_minishell(t_minishell *minishell);
void 		clear_redirs(void *redirs);
void		clean_child_data(char **matrix, char *possible_path,
						 char *part_path);
void		list_remove_if(t_list **begin_list, void *data_ref);

#endif //MINISHELL_H
