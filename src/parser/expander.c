/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:29:56 by mgrillo           #+#    #+#             */
/*   Updated: 2025/04/30 15:29:56 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "expander.h"

/*
 * Función: expand_exit_status
 * -------------------------
 * Expande la variable $? al último código de salida.
 *
 * Parámetros:
 *   str: String que contiene $?
 *   shell: Estructura principal del shell
 *
 * Retorna:
 *   Un nuevo string con $? reemplazado por el código de salida
 */
static char	*expand_exit_status(char *str, t_shell *shell)
{
	char	*result;
	char	*exit_str;
	char	*before;
	char	*after;
	char	*tmp;
	int		i;
	char	quote;

	result = ft_strdup(str);
	if (!result)
		return (NULL);

	i = 0;
	quote = 0;
	while (result && result[i])
	{
		if (result[i] == '\'' && !quote)
			quote = '\'';
		else if (result[i] == '"' && !quote)
			quote = '"';
		else if (result[i] == quote)
			quote = 0;
		else if (result[i] == '$' && result[i + 1] == '?' && quote != '\'')
		{
			before = ft_substr(result, 0, i);
			after = ft_strdup(result + i + 2);
			exit_str = ft_itoa(shell->exit_status);

			if (!before || !after || !exit_str)
			{
				free(before);
				free(after);
				free(exit_str);
				free(result);
				return (NULL);
			}

			free(result);
			result = ft_strjoin(before, exit_str);
			free(before);
			free(exit_str);

			if (!result)
			{
				free(after);
				return (NULL);
			}

			tmp = result;
			result = ft_strjoin(result, after);
			free(tmp);
			free(after);

			if (!result)
				return (NULL);

			i = ft_strlen(result) - 1;
		}
		i++;
	}

	return (result);
}

/*
 * Función: expand_env_vars
 * -----------------------
 * Expande las variables de entorno en un string.
 *
 * Parámetros:
 *   str: String que contiene variables de entorno
 *   env: Lista de variables de entorno
 *
 * Retorna:
 *   Un nuevo string con las variables expandidas
 */
char	*expand_env_vars(char *str, t_env *env)
{
	char	*result;
	char	*value;
	char	*key;
	int		i;
	int		j;
	char	quote;

	result = ft_strdup(str);
	i = 0;
	quote = 0;
	while (result[i])
	{
		if (!quote && (result[i] == '\'' || result[i] == '"'))
		{
			quote = result[i];
			i++;
			continue;
		}
		if (quote && result[i] == quote)
		{
			quote = 0;
			i++;
			continue;
		}
		if (quote == '\'')
		{
			i++;
			continue;
		}
		if (result[i] == '$')
		{
			// Si es el último carácter o seguido de caracteres no válidos, mantener el $
			if (!result[i + 1] || (!ft_isalnum(result[i + 1]) && result[i + 1] != '_' && 
				result[i + 1] != '?' && result[i + 1] != '$'))
			{
				i++;
				continue;
			}
			// Si es doble $, saltar uno
			if (result[i + 1] == '$')
			{
				char *before = ft_substr(result, 0, i);
				char *after = ft_strdup(result + i + 1);
				free(result);
				result = ft_strjoin(before, after);
				free(before);
				free(after);
				continue;
			}
			// Si es una variable válida, expandirla
			if (result[i + 1] && result[i + 1] != '?' && result[i + 1] != ' ')
			{
				j = i + 1;
				while (result[j] && (ft_isalnum(result[j]) || result[j] == '_'))
					j++;
				key = ft_substr(result, i + 1, j - i - 1);
				value = get_env_value(env, key);
				char *before = ft_substr(result, 0, i);
				char *after = ft_strdup(result + j);
				char *temp;

				// Si estamos en comillas simples, mantener el $variable
				if (quote == '\'')
				{
					char *var_name = ft_substr(result, i, j - i);
					temp = ft_strjoin(before, var_name);
					free(var_name);
				}
				// Si la variable existe, usar su valor
				else if (value)
					temp = ft_strjoin(before, value);
				// Si la variable no existe y estamos en comillas dobles, mantener solo el nombre
				else if (quote == '"')
					temp = ft_strdup(before);
				// Si la variable no existe y no estamos en comillas, usar cadena vacía
				else
					temp = ft_strdup(before);

				free(result);
				result = ft_strjoin(temp, after);
				free(before);
				free(after);
				free(temp);

				// Actualizar el índice según el caso
				if (quote == '\'')
					i += j - i - 1;
				else if (value)
					i += ft_strlen(value) - 1;
				else
					i--; // Para variables no existentes, retroceder uno para compensar el i++ del bucle

				free(key);
			}
		}
		else
			i++;
	}
	return (result);
}

/*
 * Función: expand_token
 * --------------------
 * Expande un token, incluyendo variables de entorno y $?.
 *
 * Parámetros:
 *   token: Token a expandir
 *   shell: Estructura principal del shell
 *
 * Retorna:
 *   Un nuevo string con todas las expansiones realizadas
 */
char	*expand_token(char *token, t_shell *shell)
{
	char	*result;
	char	*temp;

	if (!token)
		return (NULL);

	// Primero expandimos $?
	temp = expand_exit_status(token, shell);

	// Luego expandimos las variables de entorno
	result = expand_env_vars(temp, shell->env);
	free(temp);

	return (result);
}
