#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libraries/libft/includes/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <unistd.h>

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
 
# ifndef ERROR_PWD_ARGS
#  define ERROR_PWD_ARGS "\
zapshell: pwd: -%s: invalid option\n"
# endif

# ifndef ERROR_UNSET_ID
#  define ERROR_UNSET_ID "\
zapshell: unset: '%s': not a valid identifier\n"
# endif

# ifndef ERROR_EXPORT_ID
#  define ERROR_EXPORT_ID "\
zapshell: export: '%s': not a valid identifier\n"
# endif

# ifndef ERROR_ENV_OPTION
#  define ERROR_ENV_OPTION "\
env: invalid option -- '%c'\n"
# endif

# ifndef ERROR_ENV_MANY_ARGS
#  define ERROR_ENV_MANY_ARGS "\
env: '%s': No such file or directory\n"
# endif

# ifndef ERROR_CD_MANY_ARGS
#  define ERROR_CD_MANY_ARGS "\
zapshell: cd: too many arguments\n"
# endif

# ifndef ERROR_CD_INVALID_PATH
#  define ERROR_CD_INVALID_PATH "\
zapshell: cd: %s: Invalid file or directory\n"
# endif

# ifndef ERROR_EXIT_MANY_ARGS
#  define ERROR_EXIT_MANY_ARGS "\
exit\nzapshell: exit: too many arguments\n"
# endif

# ifndef ERROR_EXIT_INVALID_ARG
#  define ERROR_EXIT_INVALID_ARG "\
exit\nzapshell: exit: a: numeric argument required\n"
# endif

# ifndef ASCII_GREEN
#  define ASCII_GREEN "\033[0;32m%c"
# endif

# ifndef ASCII_WHITE
#  define ASCII_WHITE "\033[0m%c"
# endif

# ifndef ASCII_RESET
#  define ASCII_RESET "\033[0m%s"
# endif

# ifndef PROMPT
#  define PROMPT "ZapShell -> "
# endif

# ifndef METACHARS
#  define METACHARS "><&|()"
# endif

# ifndef METACHR_NO_AND
#  define METACHR_NO_AND "><()|"
# endif

# ifndef OPERATORS
#  define OPERATORS "><&|"
# endif

# ifndef QUOTES
#  define QUOTES "'\""
# endif

# ifndef WHITESPACE
#  define WHITESPACE " "
# endif

#ifndef TAB_OR_SPACE
# define TAB_OR_SPACE " \t"
#endif

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

void 	lexer(char *command_line, t_dlist **words);
void	parser(char *command_line, t_ast **ast);
void	execute(t_ast *ast);

void	bultin_exec(char **command, char **envp);
void	exec_echo(char **command);
int		exec_pwd(char **command);
int		exec_export(char **command, char ***envp);
void	exec_unset(char **command, char ***envp);
int		exec_env(char **command, char **envp);
void    exec_exit(char **command);
int	    exec_cd(char **command, char ***envp);

int     get_array_len(char **arr);
char	**get_env_cpy(char **envp);
int     strlen_env(char *command);
bool	isvalid_num(char *command);

//Clear functions
void		clear_token(void *token);
#endif //MINISHELL_H
