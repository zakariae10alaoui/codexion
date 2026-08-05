#include <codexion.h>

int str_parser(char *str)
{
    if (strcmp(str, "edf") != 0 && strcmp(str, "fifo") != 0)
    {
        printf("No scheduler allowed, other than fifo or edf\n");
        return 1;
    }
    return 0;
}
int int_parser(char *str)
{
    int i;
    i = 0;
    while (str[i])
    {
        if (isdigit(str[i]) || (str[i] == '-' || str[i] == '+'))
        {
            i++;
        }
        else
        {
            return 0;
            printf("argument should be an interger");
        }
    }
    return 1;
}

int ft_atoi(const char *nmb)
{
    size_t i;
    long result;
    int sign;

    i = 0;
    result = 0;
    sign = 1;
    while (nmb[i] == '+' || nmb[i] == '-' || nmb[i] == 32)
    {
        if (nmb[i] == '-')
            sign = -1;
        i++;
    }
    while (nmb[i] >= '0' && nmb[i] <= '9')
    {
        result = result * 10 + (nmb[i] - '0');
        if (result * sign > INT_MAX || result * sign < 0)
            return (-1);
        i++;
    }
    return ((int)result * sign);
}
void *parser(char **argv)
{
    int i;
    int tab[7];

    i = 1;
    while (i < 8)
    {
        if (int_parser(argv[i]))
        {
            tab[i - 1] = ft_atoi(argv[i]);
            if (tab[i - 1] == -1)
            {
                printf("Integer overflow or Negative number: %d\n", tab[i]);
                return -1;
            }
        }
        i++;
    }
    if (str_parser(argv[8]))
        return ft_fill(tab , argv[8]);
    return -1;
}
