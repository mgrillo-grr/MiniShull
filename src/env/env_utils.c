/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:29:56 by mgrillo           #+#    #+#             */
/*   Updated: 2025/04/30 15:29:56 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Función: init_env
 * ----------------
 * Inicializa la lista enlazada de variables de entorno a partir del array envp.
 *
 * Parámetros:
 *   envp: Array de strings con formato "KEY=VALUE"
 *
 * Proceso:
 * 1. Inicializa una lista vacía
 * 2. Para cada string en envp:
 *    - Separa la cadena en key y value usando el '=' como delimitador
 *    - Crea un nuevo nodo
 *    - Asigna key y value al nuevo nodo
 *    - Añade el nodo al principio de la lista
 *
 * Retorna:
 *   Puntero al primer nodo de la lista de variables de entorno
 *   NULL si hay un error de memoria
 */
t_env	*init_env(char **envp)
{
	t_env	*env;
	t_env	*new;
	char	*key;
	char	*value;
	int		i;

	env = NULL;
	i = 0;
	while (envp[i])
	{
		key = ft_substr(envp[i], 0, ft_strchr(envp[i], '=') - envp[i]);
		value = ft_strdup(ft_strchr(envp[i], '=') + 1);
		new = malloc(sizeof(t_env));
		if (!new)
			return (NULL);
		new->key = key;
		new->value = value;
		new->next = env;
		env = new;
		i++;
	}
	return (env);
}

/*
 * Función: free_env
 * ----------------
 * Libera toda la memoria asociada a la lista de variables de entorno.
 *
 * Parámetros:
 *   env: Puntero al primer nodo de la lista
 *
 * Proceso:
 * 1. Recorre la lista
 * 2. Para cada nodo:
 *    - Guarda el puntero al siguiente nodo
 *    - Libera la key y value
 *    - Libera el nodo actual
 *    - Continúa con el siguiente nodo
 */
void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = tmp;
	}
}

/*
 * Función: get_env_value
 * ---------------------
 * Busca y retorna el valor de una variable de entorno.
 *
 * Parámetros:
 *   env: Puntero al primer nodo de la lista
 *   key: Nombre de la variable a buscar
 *
 * Proceso:
 * 1. Recorre la lista de variables
 * 2. Compara cada key con la key buscada
 * 3. Si encuentra coincidencia, retorna el valor
 *
 * Retorna:
 *   Puntero al valor de la variable si existe
 *   NULL si la variable no existe
 */
char	*get_env_value(t_env *env, char *key)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (ft_strncmp(current->key, key, ft_strlen(key) + 1) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

/*
 * Función: set_env_value
 * ---------------------
 * Establece o actualiza el valor de una variable de entorno.
 *
 * Parámetros:
 *   env: Puntero al primer nodo de la lista
 *   key: Nombre de la variable
 *   value: Nuevo valor para la variable
 *
 * Proceso:
 * 1. Busca la variable en la lista
 * 2. Si existe:
 *    - Libera el valor actual
 *    - Asigna el nuevo valor
 * 3. Si no existe:
 *    - Crea un nuevo nodo
 *    - Asigna key y value
 *    - Lo añade al final de la lista
 *
 * Nota: Si hay un error de memoria, la función retorna sin hacer cambios
 */
void	set_env_value(t_env *env, char *key, char *value)
{
	t_env	*current;
	t_env	*new;

	current = env;
	while (current)
	{
		if (ft_strncmp(current->key, key, ft_strlen(key) + 1) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			return ;
		}
		if (!current->next)
			break ;
		current = current->next;
	}
	new = malloc(sizeof(t_env));
	if (!new)
		return ;
	new->key = ft_strdup(key);
	new->value = ft_strdup(value);
	new->next = NULL;
	if (current)
		current->next = new;
}
