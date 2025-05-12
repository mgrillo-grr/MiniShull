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
	t_env	*env; /* cabeza de la lista enlazada, Cuando empecemos a agregar nodos, env se irá actualizando para siempre apuntar al nodo más recientemente añadido */
	t_env	*new; /* Se usa para crear un nuevo nodo en cada iteración del bucle*/  /*cada vuelta del while, se reserva memoria con malloc para un nuevo t_env. Se llena ese nodo (key, value, next) y luego se enlaza a la lista usando env.*/
	char	*key; /* Es un puntero a una cadena de caracteres. Se usa para almacenar la parte izquierda de una variable de entorno, es decir, el nombre de la variable. */
	char	*value; /* Es otro puntero a cadena (char *), que se usa para guardar el valor asociado a la variable de entorno, o sea, la parte derecha después del '='. */
	int		i; /* Se utiliza para recorrer el array */

	env = NULL;
	i = 0;
	while (envp[i])
	{
		key = ft_substr(envp[i], 0, ft_strchr(envp[i], '=') - envp[i]); /* Busca el carácter '=' dentro de la cadena de entorno (como "PATH=/usr/bin"). Calcula cuántos caracteres hay antes del '='. Extrae esa parte (por ejemplo, "PATH") y la guarda en key. */
		value = ft_strdup(ft_strchr(envp[i], '=') + 1);  /* ft_strchr(envp[i], '=') → Encuentra el puntero al primer '=' en la cadena (ej: "PATH=/usr/bin" → apunta al '='). + 1 → Avanza un carácter, quedándose con el valor (salta el '=' y queda "usr/bin"). ft_strdup(...) → Duplica esa subcadena, reservando memoria nueva para guardarla en value */
		new = malloc(sizeof(t_env));
		if (!new)
			return (NULL);
		new->key = key; /* Guarda el nombre de la variable (por ejemplo "PATH") en el nodo new. */
		new->value = value; /* Guarda el valor asociado (por ejemplo "/usr/bin") en el mismo nodo. */
		new->next = env; /* Enlaza el nuevo nodo al principio de la lista. El puntero next de new apunta al nodo que antes era el primero. Así, el nuevo nodo queda insertado al inicio de la lista. */
		env = new; /* env siempre debe apuntar a la cabeza de la lista enlazada. Cada vez que agregás un nuevo nodo al inicio (con new->next = env), ese nuevo nodo pasa a ser la nueva cabeza. Entonces, env = new actualiza la cabeza de la lista. */
		i++;
	}
	return (env);
}
// envp[0] = "USER=juan"
// envp[1] = "HOME=/home/juan"
// envp[2] = "PATH=/usr/bin"

// ┌────────────┐     ┌────────────┐     ┌────────────┐
// │ "PATH"     │     │ "HOME"     │     │ "USER"     │
// │ "/usr/bin" │     │ "/home/juan"│    │ "juan"     │
// │ next ──────┼────▶│ next ──────┼────▶│ next = NULL│
// └────────────┘     └────────────┘     └────────────┘


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
