/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:29:56 by mgrillo           #+#    #+#             */
/*   Updated: 2025/04/30 15:29:56 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Función: cmd_echo
 * --------------
 * Implementa el comando builtin 'echo' que muestra texto en la salida estándar.
 *
 * Parámetros:
 *   args: Array de strings que contiene los argumentos del comando
 *         args[0] es "echo"
 *         args[1] en adelante son los argumentos a mostrar
 *
 * Comportamiento:
 * 1. Comprueba si existe la opción -n:
 *    - Si está presente, no se imprimirá un salto de línea al final
 *    - La opción -n debe ser el primer argumento
 *
 * 2. Imprime todos los argumentos:
 *    - Cada argumento se imprime separado por un espacio
 *    - No se añade espacio después del último argumento
 *
 * 3. Manejo del salto de línea:
 *    - Si no se usó la opción -n, añade un salto de línea al final
 *    - Si se usó -n, no añade salto de línea
 *
 * Retorna:
 *   0: Siempre retorna 0 ya que echo no puede fallar
 *
 * Ejemplo de uso:
 *   echo hola mundo     -> imprime: hola mundo\n
 *   echo -n hola mundo  -> imprime: hola mundo
 */
int	cmd_echo(t_shell *shell, char **args)
{
	int	i;
	int	newline;

	(void)shell;
	newline = 1;
	i = 1;

	if (args[1] && ft_strncmp(args[1], "-n", 3) == 0)
	{
		newline = 0;
		i++;
	}

	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	/* Añade salto de línea si no se usó la opción -n */
	if (newline)
		ft_putchar_fd('\n', 1);
	return (0);
}
