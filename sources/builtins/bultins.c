#include "../../includes/minishell.h"

static void	exec_echo(char **command);
static int	exec_pwd(char **command);
static void	exec_export(char **command, char **envp);
static int	exec_env(char **command, char **envp);
static char	**ft_get_env(char **envp);
static bool	ft_isnumber(char *string);

void	bultin_exec(char **command, char **envp)
{
    char	**envp_cpy;

    envp_cpy = ft_get_env(envp);
    if (!ft_strncmp(command[0], "echo", 5))
        exec_echo(command);
    else if (!ft_strncmp(command[0], "cd", 3))
        exec_cd(command, &envp_cpy);
    else if (!ft_strncmp(command[0], "pwd", 4))
        exec_pwd(command);
    else if (!ft_strncmp(command[0], "export", 7))
        exec_export(command, envp_cpy);
    else if (!ft_strncmp(command[0], "unset", 6))
        exec_unset(command, &envp_cpy);
    else if (!ft_strncmp(command[0], "env", 4))
        exec_env(command, envp_cpy);
    else if (!ft_strncmp(command[0], "exit", 5))
        exec_exit(command);
}

void    exec_exit(char **command)
{
    int arr_len;
    int status;

    arr_len = get_array_len(command);
    if (arr_len == 1)
    {
        printf("exit\n");
        status = 0;
    }
    else
    {
        if (arr_len == 2 && ft_isvalid_num(command[1]))
        {
            printf("exit\n");
            status = atoll(command[1]) % 256;
        }
        else if (arr_len != 2 && ft_isvalid_num(command[1]))
        {
            printf("exit\nzapshell: exit: too many arguments\n");
            return;
        }
        else
        {
            printf("exit\nzapshell: exit: a: numeric argument required\n");
            status = 2;
        }
    }
    //limpar memoria alocada ate agora e validar o que fazer com status quando nao dar exit.
    exit(status);
}

void	exec_echo(char **command) {
    int arr_len;
    int i;
    int x;
    int y;

    arr_len = get_array_len(command);
    i = 0;
    if (arr_len == 1)
    {
        printf("\n");
        return;
    }
    if (command[1][0] == '-' && command[1][1] == 'n')
    {
        i = 1;
        y = 2;
        while (command[1][y])
        {
            if (command[1][y] != 'n') {
                i = 0;
                break;
            }
            y++;
        }
    }
    if (arr_len > 1)
    {
        if (i == 1)
        {
            x = 2;
            while(arr_len > x)
            {
                printf("%s", command[x]);
                if (x + 1 != arr_len)
                    printf(" ");
                x++;
            }
        }
        else
        {
            x = 1;
            while (arr_len > x)
            {
                printf("%s", command[x]);
                if (x + 1 != arr_len)
                    printf(" ");
                x++;
            }
            printf("\n");
        }
    }
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

char	**ft_get_env(char **envp)
{
    int count;
    int i;
    char **temp;

    count = 0;
    i = 0;
    while(envp[count])
        count++;
    temp = malloc(sizeof(char *) * count + 1);
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