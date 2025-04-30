/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:29:56 by mgrillo           #+#    #+#             */
/*   Updated: 2025/04/30 15:29:56 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Función: cmd_env
 * ---------------
 * Implementa el comando builtin 'env' que muestra las variables de entorno.
 *
 * Parámetros:
 *   env: Lista enlazada de variables de entorno
 *
 * Comportamiento:
 * 1. Recorre la lista de variables de entorno
 * 2. Para cada variable:
 *    - Si tiene un valor asignado (no es NULL):
 *      * Muestra el nombre de la variable
 *      * Muestra el signo igual (=)
 *      * Muestra el valor
 *      * Añade un salto de línea
 *    - Si no tiene valor, la variable se omite
 *
 * Formato de salida:
 *   NOMBRE=valor
 *
 * Ejemplos:
 *   PATH=/usr/local/bin:/usr/bin
 *   HOME=/home/usuario
 *   LANG=es_ES.UTF-8
 *
 * Diferencias con 'export':
 * - env solo muestra variables con valor asignado
 * - No usa el formato 'declare -x'
 * - No muestra comillas en los valores
 *
 * Retorna:
 *   0: Siempre retorna 0 (env no puede fallar)
 */
int	cmd_env(t_shell *shell, char **args)
{
	t_env	*current;

	(void)args;
	current = shell->env;
	while (current)
	{
		/* Solo muestra variables que tengan un valor */
		if (current->value)
		{
			ft_putstr_fd(current->key, 1);
			ft_putchar_fd('=', 1);
			ft_putendl_fd(current->value, 1);
		}
		current = current->next;
	}
	return (0);
}
