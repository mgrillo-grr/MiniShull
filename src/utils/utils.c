/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:29:56 by mgrillo           #+#    #+#             */
/*   Updated: 2025/04/30 15:29:56 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Función: ft_strcmp
 * ------------------
 * Compara dos strings.
 */
int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

/*
 * Función: remove_quotes
 * ---------------------
 * Remueve las comillas simples y dobles de un string.
 */
t_quote_info	*remove_quotes(char *str)
{
	t_quote_info	*info;
	char	*result;
	int		i;
	int		j;
	char	outer_quote;

	info = malloc(sizeof(t_quote_info));
	if (!info)
		return (NULL);
	result = malloc(ft_strlen(str) + 1);
	if (!result)
	{
		free(info);
		return (NULL);
	}

	// Si es "" o '', devolver cadena vacía
	if (ft_strlen(str) == 2 && 
		((str[0] == '"' && str[1] == '"') || 
		 (str[0] == '\'' && str[1] == '\''))) 
	{
		result[0] = '\0';
		info->str = result;
		info->has_single = (str[0] == '\'');
		info->has_double = (str[0] == '"');
		return info;
	}

	i = 0;
	j = 0;
	outer_quote = 0;
	info->has_single = 0;
	info->has_double = 0;

	while (str[i])
	{
		// Si encontramos una comilla simple
		if (!outer_quote && str[i] == '\'')
		{
			outer_quote = '\'';
			info->has_single = 1;
		}
		// Si encontramos una comilla doble
		else if (!outer_quote && str[i] == '"')
		{
			outer_quote = '"';
			info->has_double = 1;
		}
		// Si encontramos una comilla que cierra
		else if (outer_quote && str[i] == outer_quote)
			outer_quote = 0;
		// Si estamos dentro de comillas simples, mantener todo literal
		else if (outer_quote == '\'')
			result[j++] = str[i];
		// Si no hay comillas o estamos en comillas dobles, copiar el caracter
		else
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';

	info->str = result;
	return (info);
}

/*
 * Función: add_token
 * ------------------
 * Añade un nuevo token a la lista de tokens.
 */
char	**add_token(char **tokens, int *token_count, char *str, int len)
{
	char	**new_tokens;
	int		i;

	if (!str || !len)
		return (tokens);

	new_tokens = malloc(sizeof(char *) * (*token_count + 2));
	if (!new_tokens)
		return (NULL);

	for (i = 0; i < *token_count; i++)
		new_tokens[i] = tokens[i];

	new_tokens[*token_count] = ft_strdup(str);
	if (!new_tokens[*token_count])
	{
		free(new_tokens);
		return (NULL);
	}

	new_tokens[*token_count + 1] = NULL;
	(*token_count)++;

	return (new_tokens);
}

/*
 * Función: free_array
 * ------------------
 * Libera la memoria de un array de strings.
 */
void	free_array(char **array)
{
	int	i;

	if (!array)
		return;

	i = 0;
	while (array[i])
	{
		if (array[i])
			free(array[i]);
		array[i] = NULL;
		i++;
	}
	free(array);
}

/*
 * Función: error_msg
 * ----------------
 * Imprime un mensaje de error prefijado con "minishell: ".
 *
 * Parámetros:
 *   msg: Mensaje de error a imprimir
 */
void	error_msg(char *msg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putendl_fd(msg, 2);
}
