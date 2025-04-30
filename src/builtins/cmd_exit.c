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

/*
 * Función: is_numeric
 * ------------------
 * Verifica si una cadena representa un número válido.
 *
 * Parámetros:
 *   str: Cadena a verificar
 *
 * Reglas de validación:
 * 1. Puede comenzar con un signo opcional (+ o -)
 * 2. El resto de caracteres deben ser dígitos (0-9)
 * 3. No se permiten espacios ni otros caracteres
 *
 * Ejemplos válidos:
 *   "123"   -> retorna 1
 *   "-456"  -> retorna 1
 *   "+789"  -> retorna 1
 *
 * Ejemplos inválidos:
 *   "12.3"  -> retorna 0
 *   "abc"   -> retorna 0
 *   "1a2b3" -> retorna 0
 *
 * Retorna:
 *   1: La cadena es un número válido
 *   0: La cadena no es un número válido
 */
static int	is_numeric(char *str)
{
	int	i;

	i = 0;
	/* Maneja el signo opcional */
	if (str[i] == '-' || str[i] == '+')
		i++;
	/* Verifica que todos los caracteres sean dígitos */
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
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
int	cmd_exit(t_shell *shell, char **args)
{
	int	exit_code;

	/* Muestra el mensaje "exit" */
	ft_putendl_fd("exit", 2);

	/* Sin argumentos: sale con código 0 */
	if (!args[1])
		exit(shell->exit_status);

	/* Verifica que el argumento sea numérico */
	if (!is_numeric(args[1]))
	{
		ft_putendl_fd(ERR_PREFIX"exit"ERR_NUM_ARG, 2);
		exit(255);
	}

	/* Convierte el argumento a número */
	exit_code = ft_atoi(args[1]);

	/* Verifica si hay demasiados argumentos */
	if (args[2])
	{
		ft_putendl_fd(ERR_EXIT_ARGS, 2);
		return (1);
	}

	/* Sale con el código especificado */
	exit(exit_code % 256);
}
