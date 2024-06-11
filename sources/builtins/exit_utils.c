#include "../../includes/minishell.h"

static bool	ft_isnumber(char *string);
static char *lltoa(long long c);
static void ft_putnbr_str(long long n, size_t i, size_t len, char *str);
static size_t   ft_ilen(long long number);

bool	ft_isvalid_num(char *command)
{
    long long c;
    char *string;

    if (command[0] == '+' || command[0] == '-')
        command++;
    if (ft_isnumber(command))
    {
        c = atoll(command);
        string = lltoa(c);
        if (strcmp(string, command) == 0) {
            free(string);
            return (true);
        }
        else
        {
            free(string);
            return (false);
        }
    }
    else
        return (false);
}

static bool	ft_isnumber(char *string)
{
    int	i;

    i = 0;
    while (string[i])
    {
        if (!ft_isdigit(string[i]))
            return (false);
        i++;
    }
    return (true);
}

static char *lltoa(long long c)
{
    char *string;
    size_t length;
    long long int number;

    number = c;
    length = ft_ilen(number);
    string = ft_calloc(length + 1,sizeof(char));
    if (!string)
        return (NULL);
    if (number < 0)
    {
        number *= -1;
        *string = '-';
        ft_putnbr_str(number, 1, length, string);
    }
    else
        ft_putnbr_str(number, 0, length, string);
    return (string);
}

static void	ft_putnbr_str(long long n, size_t i, size_t len, char *str)
{
    while (i < len)
    {
        str[--len] = (n % 10) + '0';
        n /= 10;
    }
}

static size_t	ft_ilen(long long number)
{
    size_t	counter;

    counter = 0;
    if (number <= 0)
    {
        number *= -1;
        counter++;
    }
    while (number)
    {
        number /= 10;
        counter++;
    }
    return (counter);
}
