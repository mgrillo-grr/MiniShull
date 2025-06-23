/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:29:56 by mgrillo           #+#    #+#             */
/*   Updated: 2025/04/30 15:29:56 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Función: is_valid_identifier
 * ---------------------------
 * Verifica si una cadena es un identificador válido para una variable de entorno.
 *
 * Parámetros:
 *   str: Cadena a verificar
 *
 * Reglas de validación:
 * 1. Debe comenzar con una letra o guión bajo (_)
 * 2. Los caracteres siguientes deben ser:
 *    - Letras (a-z, A-Z)
 *    - Números (0-9)
 *    - Guión bajo (_)
 * 3. La validación se detiene al encontrar un signo igual (=)
 *
 * Retorna:
 *   1: El identificador es válido
 *   0: El identificador no es válido
 */
int	is_valid_identifier(const char *str)
{
	int	i;

	/* Verifica que el primer carácter sea una letra o guión bajo */
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);

	/* Verifica el resto de caracteres hasta el final o hasta el '=' */
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

/*
 * Función: print_export
 * --------------------
 * Muestra todas las variables de entorno en formato 'export'.
 *
 * Parámetros:
 *   env: Lista enlazada de variables de entorno
 *
 * Formato de salida:
 * - Cada variable se muestra como: declare -x NOMBRE="VALOR"
 * - Si la variable no tiene valor, solo se muestra: declare -x NOMBRE
 * - Los valores se muestran entre comillas dobles
 *
 * Ejemplo de salida:
 *   declare -x PATH="/usr/local/bin:/usr/bin"
 *   declare -x HOME="/home/usuario"
 *   declare -x LANG="es_ES.UTF-8"
 */
static void	print_export(t_env *env)
{
	t_env	*current;

	current = env;
	while (current)
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(current->key, 1);
		if (current->value)
		{
			ft_putchar_fd('=', 1);
			ft_putchar_fd('"', 1);
			ft_putstr_fd(current->value, 1);
			ft_putchar_fd('"', 1);
		}
		ft_putchar_fd('\n', 1);
		current = current->next;
	}
}

/*
 * Función: cmd_export
 * -----------------
 * Implementa el comando builtin 'export' para definir variables de entorno.
 *
 * Parámetros:
 *   args: Array de strings con los argumentos
 *         args[0] es "export"
 *         args[1..n] son las variables a exportar
 *   env: Lista enlazada de variables de entorno
 *
 * Comportamiento:
 * 1. Sin argumentos:
 *    - Muestra todas las variables en formato 'export'
 *
 * 2. Con argumentos:
 *    - Para cada argumento:
 *      a. Verifica que sea un identificador válido
 *      b. Si contiene '=', separa en nombre y valor
 *      c. Añade o actualiza la variable en el entorno
 *
 * Formato de argumentos válidos:
 *   export NOMBRE=valor
 *   export NOMBRE="valor con espacios"
 *   export NOMBRE=
 *   export NOMBRE
 *
 * Retorna:
 *   0: Éxito
 *   1: Error - identificador inválido
 */
int	cmd_export(t_shell *shell, char **args)
{
	int		i;
	char	*key;
	char	*value;
	char	*equal_sign;

	/* Si no hay argumentos, muestra todas las variables */
	if (!args[1])
	{
		print_export(shell->env);
		return (0);
	}

	/* Procesa cada argumento */
	i = 1;
	while (args[i])
	{
		/* Verifica que el identificador sea válido */
		if (!is_valid_identifier(args[i]))
		{
			ft_putendl_fd(ERR_EXPORT_ARGS, 2);
			return (1);
		}

		/* Busca el signo igual para separar nombre y valor */
		equal_sign = ft_strchr(args[i], '=');
		if (equal_sign)
		{
			key = ft_substr(args[i], 0, equal_sign - args[i]);
			value = ft_strdup(equal_sign + 1);
			set_env_value(shell->env, key, value);
			free(key);
			free(value);
		}
		i++;
	}
	return (0);
}
