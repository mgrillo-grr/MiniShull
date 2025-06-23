/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:29:56 by mgrillo           #+#    #+#             */
/*   Updated: 2025/04/30 15:29:56 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <limits.h>

static int	ft_isspace(int c)
{
	return (c == ' ' || (c >= '\t' && c <= '\r'));
}


/*
 * Función: cmd_exit
 * ----------------
 * Implementa el comando builtin 'exit' para terminar la ejecución del shell.
 *
 * Parámetros:
 *   args: Array de strings con los argumentos
 *         args[0] es "exit"
 *         args[1] es el código de salida opcional
 *
 * Comportamiento:
 * 1. Muestra "exit" en stderr
 *
 * 2. Sin argumentos:
 *    - Sale con código 0
 *
 * 3. Con un argumento:
 *    - Si es numérico:
 *      * Sale con ese código de salida
 *    - Si no es numérico:
 *      * Muestra error
 *      * Sale con código 255
 *
 * 4. Con más de un argumento:
 *    - Si el primero no es numérico:
 *      * Comportamiento igual que con un argumento no numérico
 *    - Si el primero es numérico:
 *      * Muestra error "too many arguments"
 *      * NO sale del shell
 *      * Retorna 1
 *
 * Códigos de salida:
 *   0: Salida normal (sin argumentos)
 *   255: Error en el argumento numérico
 *   N: Valor especificado por el usuario (0-255)
 *   1: Error por demasiados argumentos (no sale)
 */
static int	is_valid_number(const char *str)
{
	int	i;
	int	has_digit;
	int	in_quotes;
	int	sign_pos;

	i = 0;
	has_digit = 0;
	in_quotes = 0;
	sign_pos = -1;

	/* Skip leading spaces */
	while (ft_isspace(str[i]))
		i++;

	/* Handle sign before quote */
	if (str[i] == '+' || str[i] == '-')
	{
		sign_pos = i;
		i++;
	}

	/* Handle opening quote */
	if (str[i] == '\'' || str[i] == '"')
	{
		in_quotes = str[i];
		i++;
		/* If sign was before quote, it's invalid */
		if (sign_pos != -1 && sign_pos != i - 2)
			return (0);
	}

	/* Handle sign inside quotes */
	if ((str[i] == '+' || str[i] == '-') && in_quotes)
		i++;

	/* Check for at least one digit */
	if (!ft_isdigit(str[i]))
	{
		if (sign_pos != -1 || in_quotes)
			return (0);
		return (0);
	}

	/* Check remaining digits */
	while (ft_isdigit(str[i]))
	{
		has_digit = 1;
		i++;
	}

	/* If we're in quotes, skip to closing quote */
	if (in_quotes)
	{
		if (str[i] != in_quotes)
			return (0);
		i++;
	}

	/* Skip trailing spaces */
	while (ft_isspace(str[i]))
		i++;

	/* Check if we have any trailing characters */
	if (str[i] != '\0')
		return (0);

	return (has_digit);
}

static unsigned char	get_exit_code(const char *str)
{
	long long	num;
	int		sign;
	int		i;
	int		in_quotes;

	num = 0;
	sign = 1;
	i = 0;
	in_quotes = 0;

	/* Skip leading spaces */
	while (ft_isspace(str[i]))
		i++;

	/* Handle sign before quote */
	if (str[i] == '+' || str[i] == '-')
	{
		sign = (str[i] == '-') ? -1 : 1;
		i++;
	}

	/* Handle opening quote */
	if (str[i] == '\'' || str[i] == '"')
	{
		in_quotes = str[i];
		i++;
		/* If sign is after opening quote, handle it again */
		if (str[i] == '+' || str[i] == '-')
		{
			sign = (str[i] == '-') ? -1 : 1;
			i++;
		}
	}

	/* Convert number */
	while (ft_isdigit(str[i]))
	{
		num = num * 10 + (str[i] - '0');
		i++;
	}

	/* Skip closing quote if any */
	if (in_quotes && str[i] == in_quotes)
		i++;

	/* Calculate exit code */
	if (sign == -1)
	{
		num = (256 - (num % 256)) % 256;
	}
	else
	{
		num = num % 256;
	}

	return ((unsigned char)num);
}

static int	has_too_many_args(char **args)
{
	int	count;

	count = 0;
	while (args[count])
		count++;
	return (count > 2);
}

int	cmd_exit(t_shell *shell, char **args)
{
	/* Muestra el mensaje "exit" solo si estamos en modo interactivo */
	if (isatty(STDIN_FILENO))
		ft_putendl_fd("exit", 2);

	/* Sin argumentos: sale con el último estado de salida */
	if (!args[1])
		exit(shell->exit_status);

	/* Verifica si hay demasiados argumentos */
	if (has_too_many_args(args))
	{
		ft_putstr_fd(ERR_PREFIX"exit: ", 2);
		ft_putendl_fd("too many arguments", 2);
		return (1);
	}

	/* Verifica si el argumento es numérico */
	if (!is_valid_number(args[1]))
	{
		ft_putstr_fd(ERR_PREFIX"exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		exit(2);
	}

	/* Obtiene el código de salida y sale */
	exit(get_exit_code(args[1]));
}
