/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_array.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:29:56 by mgrillo           #+#    #+#             */
/*   Updated: 2025/04/30 15:29:56 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Función: env_size
 * ----------------
 * Cuenta el número de variables en la lista de entorno.
 *
 * Parámetros:
 *   env: Lista enlazada de variables de entorno
 *
 * Retorna:
 *   Número de variables en la lista
 */
static int	env_size(t_env *env)
{
	int		size;
	t_env	*current;

	size = 0;
	current = env;
	while (current)
	{
		size++;
		current = current->next;
	}
	return (size);
}

/*
 * Función: env_to_array
 * -------------------
 * Convierte la lista enlazada de variables de entorno en un array de strings.
 *
 * Parámetros:
 *   env: Lista enlazada de variables de entorno
 *
 * Proceso:
 * 1. Cuenta el número de variables
 * 2. Crea un array del tamaño necesario
 * 3. Para cada variable:
 *    - Concatena key, "=" y value
 *    - Guarda el resultado en el array
 *
 * Retorna:
 *   Array de strings con las variables de entorno (formato KEY=VALUE)
 *   NULL si hay un error de memoria
 */
char	**env_to_array(t_env *env)
{
	char	**array;
	char	*tmp;
	t_env	*current;
	int		i;

	array = malloc(sizeof(char *) * (env_size(env) + 1));
	if (!array)
		return (NULL);
	current = env;
	i = 0;
	while (current)
	{
		tmp = ft_strjoin(current->key, "=");
		array[i] = ft_strjoin(tmp, current->value);
		free(tmp);
		if (!array[i])
		{
			while (--i >= 0)
				free(array[i]);
			free(array);
			return (NULL);
		}
		current = current->next;
		i++;
	}
	array[i] = NULL;
	return (array);
}
