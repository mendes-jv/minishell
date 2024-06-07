#include "../includes/minishell.h"

static void	bultin_exec(char **command, char **envp);
static void	exec_echo(char **command);
static int	exec_cd(char **command, char **envp);
static int	exec_pwd(char **command);
static void	exec_export(char **command, char **envp);
static void	exec_unset(char **command, char **envp);
static int	exec_env(char **command, char **envp);
static int  get_biggest_len(char *old_path, char *new_path);
static char	*get_home_dir(char **envp);
static char	**ft_get_env(char **envp);
static int get_array_len(char **arr);
static void exec_exit(char **command);

void	bultin_exec(char **command, char **envp)
{
    char	**envp_cpy;

    envp_cpy = ft_get_env(envp);
    if (!ft_strncmp(command[0], "echo", 5))
        exec_echo(command);
    else if (!ft_strncmp(command[0], "cd", 3))
        exec_cd(command, envp_cpy);
    else if (!ft_strncmp(command[0], "pwd", 4))
        exec_pwd(command);
    else if (!ft_strncmp(command[0], "export", 7))
        exec_export(command, envp_cpy);
    else if (!ft_strncmp(command[0], "unset", 6))
        exec_unset(command, command);
    else if (!ft_strncmp(command[0], "env", 4))
        exec_env(command, envp_cpy);
    else if (!ft_strncmp(command[0], "exit", 5))
        exec_exit(command);
}

void    exec_exit(char **command)
{
    if (get_array_len(command) == 1)
    {
        //precisa limpar memoria alocada utilizada no momento da execução
        exit(0);
    }
    //validar se é o command[1] é numerico. se sim, retornar esse valor. se nao, retornar 2 com mensagem especifica.
}

void	exec_echo(char **command)
{
    if (command[0] == '-'&& command[1] == 'n')
    {
        command += 2;
        ft_printf("%s", command);
    }
    else
        ft_printf("%s\n", command);
}

int	exec_cd(char **command, char **envp)
{
    //preciso ainda validar se altero o oldpwd e o pwd na env com o chdir automaticamente;
    char *old_path;
    char *new_path;
    int exit_status;

    exit_status = 0;
    if (command != NULL)
    {
        old_path = getcwd(NULL, 0);
        chdir(command);
        new_path = getcwd(NULL, 0);
        if (!ft_strncmp(old_path, new_path, get_biggest_len(old_path, new_path)))
        {
            ft_printf("zapshell: cd: %s: Invalid file or directory\n", command);
            exit_status = 1;
        }
        free(old_path);
        free(new_path);
    }
    else
    {
        new_path = get_home_dir(envp);
        chdir(new_path);
    }
    return(exit_status);
}

int	exec_pwd(char **command)
{
    char	*cwd;

    if (get_array_len(command) > 1)
    {
        if (command[1][0] == '-')
            return (printf("zapshell: pwd: -%s: invalid option\n", command[1]) ,2);
    }
    cwd = getcwd(NULL, 0);
    if (!cwd)
        return(1);
    printf("%s\n", cwd);
    free(cwd);
    return 0;
}

void	exec_export(char **command, char **envp)
{
    int	count;
    int	i;
    char	**temp;

    count = 0;
    i = 0;
    if (command)
    {
        while(envp[count])
            count++;
        temp = malloc(sizeof(char *) * (count + 1));
        while(count > i)
        {
            temp[i] = ft_strdup(envp[i]);
            i++;
        }
        temp[i] = ft_strdup(command);
        free(envp);
    }
    else if (!command)
    {
        //printar em ordem alfabeto ascii;
    }
    //precisa atualizar o ponteiro do env
}

void	exec_unset(char **command, char **envp)
{
    char *temp;
    int	count;
    int	i;

    count = 0;
    i = 0;
    while(envp[count])
        count++;
    temp = malloc(sizeof(char) * (count - 1));
    while(count > i)
    {
        if (ft_strncmp(envp[i], get_env(command), ft_strlen(envp[i])))
            temp[i] = ft_strdup(envp[i]);
        i++;
    }
    free(envp);
    //precisa atualizar o ponteiro do env
}

int	exec_env(char **command, char **envp)
{
    int	count;

    count = 0;
    if (get_array_len(command) > 1)
    {
        if (command[1][0] == '-')
            return (printf("env: invalid option -- '%c'\n", command[1][1]), 125);
        else
            return (printf("env: '%s': No such file or directory\n", command[1]), 127);
    }
    while (envp[count])
    {
        printf("%s\n", envp[count]);
        count++;
    }
    return 0;
}

int     get_biggest_len(char *old_path, char *new_path)
{
    if (strlen(new_path) >= strlen(old_path))
        return(strlen(new_path));
    else
        return(strlen(old_path));
}

char	*get_home_dir(char **envp)
{
    char	*home;

    while (envp && ft_strncmp (*envp, "HOME=", 5))
        envp++;
    home = *envp + 5;
    return (home);
}

char	**ft_get_env(char **envp)
{
    int count;
    int i;
    char **temp;

    count = 0;
    i = 0;
    while(envp[count])
        count++;
    temp = malloc(sizeof(char *) * count);
    while(count > i)
    {
        temp[i] = ft_strdup(envp[i]);
        i++;
    }
    return(temp);
}

int     get_array_len(char **arr)
{
    int arr_size;

    arr_size = 0;
    while (arr[arr_size])
        arr_size++;
    return (arr_size);
}