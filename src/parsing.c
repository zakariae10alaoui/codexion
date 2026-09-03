#include "codexion.h"

int	str_parser(char *str)
{
	if (strcmp(str, "edf") != 0 && strcmp(str, "fifo") != 0)
	{
		printf("No scheduler allowed, other than fifo or edf\n");
		return (0);
	}
	return (1);
}

int	int_parser(char *str)
{
	int	i;

	i = 0;
	if (str[0] == '\0')
	{
		printf("argument should be an integer\n");
		return (0);
	}
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (str[i] == '\0')
	{
		printf("argument should be an integer\n");
		return (0);
	}
	while (str[i])
	{
		if (!isdigit((unsigned char)str[i]))
		{
			printf("argument should be an integer\n");
			return (0);
		}
		i++;
	}
	return (1);
}

int	ft_atoi(const char *nmb)
{
	size_t	i;
	long	result;
	int		sign;

	i = 0;
	result = 0;
	sign = 1;
	if (nmb[i] == '-' || nmb[i] == '+')
	{
		if (nmb[i] == '-')
			sign = -1;
		i++;
	}
	while (nmb[i] >= '0' && nmb[i] <= '9')
	{
		result = result * 10 + (nmb[i] - '0');
		if (result * sign > INT_MAX || result * sign < INT_MIN)
			return (-1);
		i++;
	}
	return ((int)result * sign);
}

void	tmp_fill(s_args *data, int *tab, char *str)
{

	data->number_of_coders = tab[0];
	data->time_to_burnout = tab[1];
	data->time_to_compile = tab[2];
	data->time_to_debug = tab[3];
	data->time_to_refactor = tab[4];
	data->number_of_compiles_required = tab[5];
	data->dongle_cooldown = tab[6];
	data->scheduler = str;
}

s_args	*parser(char **argv)
{
	int		i;
	int		tab[7];
	s_args	*data;

	i = 1;
	while (i < 8)
	{
		if (!int_parser(argv[i]))
			return (NULL);
		tab[i - 1] = ft_atoi(argv[i]);
		if (tab[i - 1] < 0)
		{
			printf("Integer overflow or negative number: %s\n", argv[i]);
			return (NULL);
		}
		i++;
	}

	if (!str_parser(argv[8]))
		return (NULL);
	data = malloc(sizeof(s_args));
	if (!data)
		return (NULL);
	tmp_fill(data, tab, argv[8]);
	return (data);
}
