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
	int		i;
	char	quote;

	result = ft_strdup(str);
	i = 0;
	quote = 0;
	while (result[i])
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

			free(result);
			result = ft_strjoin(before, exit_str);
			result = ft_strjoin(result, after);

			free(before);
			free(after);
			free(exit_str);
			i += ft_strlen(exit_str) - 1;
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
static char	*expand_env_vars(char *str, t_env *env)
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
		if (result[i] == '$' && result[i + 1] && 
			result[i + 1] != '?' && result[i + 1] != ' ')
		{
			j = i + 1;
			while (result[j] && (ft_isalnum(result[j]) || result[j] == '_'))
				j++;
			key = ft_substr(result, i + 1, j - i - 1);
			value = get_env_value(env, key);
			if (value)
			{
				char *before = ft_substr(result, 0, i);
				char *after = ft_strdup(result + j);
				char *temp = ft_strjoin(before, value);
				free(result);
				result = ft_strjoin(temp, after);
				free(before);
				free(after);
				free(temp);
				i += ft_strlen(value) - 1;
			}
			free(key);
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
