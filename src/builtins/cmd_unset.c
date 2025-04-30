/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:29:56 by mgrillo           #+#    #+#             */
/*   Updated: 2025/04/30 15:29:56 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Función: remove_env_var
 * ----------------------
 * Elimina una variable de entorno de la lista enlazada.
 *
 * Parámetros:
 *   env: Puntero doble a la lista de variables de entorno
 *   key: Nombre de la variable a eliminar
 *
 * Comportamiento:
 * 1. Caso especial - lista vacía:
 *    - Retorna sin hacer nada
 *
 * 2. Caso especial - primer nodo:
 *    - Si la variable está en el primer nodo:
 *      * Actualiza el puntero inicial
 *      * Libera la memoria del nodo
 *
 * 3. Caso general:
 *    - Busca la variable en la lista
 *    - Si la encuentra:
 *      * Actualiza los enlaces
 *      * Libera la memoria del nodo
 *
 * Gestión de memoria:
 * - Libera la memoria de:
 *   * La clave (key)
 *   * El valor (value)
 *   * El nodo en sí
 */
static void	remove_env_var(t_env **env, char *key)
{
	t_env	*current;
	t_env	*prev;

	/* Caso: lista vacía */
	if (!*env)
		return ;

	/* Caso: variable en el primer nodo */
	if (ft_strncmp((*env)->key, key, ft_strlen(key) + 1) == 0)
	{
		current = *env;
		*env = (*env)->next;
		free(current->key);
		free(current->value);
		free(current);
		return ;
	}

	/* Caso general: buscar en el resto de la lista */
	current = *env;
	prev = NULL;
	while (current && ft_strncmp(current->key, key, ft_strlen(key) + 1) != 0)
	{
		prev = current;
		current = current->next;
	}

	/* Si se encontró la variable, eliminarla */
	if (current)
	{
		prev->next = current->next;
		free(current->key);
		free(current->value);
		free(current);
	}
}

/*
 * Función: cmd_unset
 * -----------------
 * Implementa el comando builtin 'unset' para eliminar variables de entorno.
 *
 * Parámetros:
 *   args: Array de strings con los argumentos
 *         args[0] es "unset"
 *         args[1..n] son los nombres de variables a eliminar
 *   env: Puntero doble a la lista de variables de entorno
 *
 * Comportamiento:
 * 1. Sin argumentos:
 *    - No hace nada, retorna 0
 *
 * 2. Con argumentos:
 *    - Para cada argumento:
 *      * Intenta eliminar la variable correspondiente
 *      * Si la variable no existe, continúa sin error
 *
 * Ejemplos de uso:
 *   unset PATH      -> elimina la variable PATH
 *   unset VAR1 VAR2 -> elimina VAR1 y VAR2
 *   unset NOEXISTE  -> no hace nada
 *
 * Retorna:
 *   0: Siempre retorna 0 (el comando unset no falla)
 */
int	cmd_unset(t_shell *shell, char **args)
{
	int	i;

	/* Si no hay argumentos, retorna sin hacer nada */
	if (!args[1])
		return (0);

	/* Procesa cada argumento */
	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("unset: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putendl_fd("': not a valid identifier", 2);
		}
		else
			remove_env_var(&shell->env, args[i]);
		i++;
	}
	return (0);
}
