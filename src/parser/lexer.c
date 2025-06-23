/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:29:56 by mgrillo           #+#    #+#             */
/*   Updated: 2025/04/30 15:29:56 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Función: is_special_char
 * ------------------------
 * Verifica si un carácter es especial (pipe o redirección).
 */
static int	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

/*
 * Función: skip_spaces
 * --------------------
 * Avanza el índice hasta el siguiente carácter no espacio.
 */
static void	skip_spaces(char *str, int *i)
{
	while (str[*i] && (str[*i] == ' ' || str[*i] == '\t'))
		(*i)++;
}

/*
 * Función: get_token_length
 * ------------------------
 * Obtiene la longitud del siguiente token.
 */
static int	get_token_length(char *str, int start)
{
	int	len;
	char	quote;

	len = 0;
	quote = 0;

	if (is_special_char(str[start]))
		return (1);

	while (str[start + len])
	{
		// Si encontramos una comilla que no está escapada
		if ((str[start + len] == '\'' || str[start + len] == '"') 
			&& (len == 0 || str[start + len - 1] != '\\'))
		{
			if (quote == 0)
				quote = str[start + len]; // Abrir comilla
			else if (quote == str[start + len])
				quote = 0; // Cerrar comilla
		}
		// Si no estamos dentro de comillas y encontramos un delimitador
		else if (quote == 0 && (is_special_char(str[start + len]) || 
			str[start + len] == ' ' || str[start + len] == '\t'))
		{
			break;
		}
		len++;
	}
	return (len);
}



/*
 * Función: tokenize
 * -----------------
 * Divide la entrada en tokens, separando por espacios y caracteres especiales.
 */
static int	check_unclosed_quotes(char *input)
{
	int		i;
	char	quote;
	int		count_single;
	int		count_double;

	i = 0;
	quote = 0;
	count_single = 0;
	count_double = 0;
	while (input[i])
	{
		if (!quote && input[i] == '\'')
		{
			quote = '\'';
			count_single++;
		}
		else if (!quote && input[i] == '"')
		{
			quote = '"';
			count_double++;
		}
		else if (quote == '\'' && input[i] == '\'')
		{
			quote = 0;
			count_single++;
		}
		else if (quote == '"' && input[i] == '"')
		{
			quote = 0;
			count_double++;
		}
		i++;
	}
	return (count_single % 2 != 0 || count_double % 2 != 0);
}

char	**tokenize(char *input)
{
	char	**tokens;
	char	**new_tokens;
	int		token_count;
	int		i;
	int		len;

	if (!input || check_unclosed_quotes(input))
		return (NULL);

	tokens = NULL;
	token_count = 0;
	i = 0;

	while (input[i])
	{
		skip_spaces(input, &i);
		if (!input[i])
			break;

		// Manejar operadores de redirección dobles (>>, <<)
		if ((input[i] == '>' && input[i + 1] == '>') || 
			(input[i] == '<' && input[i + 1] == '<'))
		{
			len = 2; // Longitud fija de 2 para los operadores dobles
		}
		// Manejar operadores de redirección simples (<, >) que no son parte de un operador doble
		else if (is_special_char(input[i]) && 
			   !((input[i] == '>' && input[i + 1] == '>') || 
			     (input[i] == '<' && input[i + 1] == '<')))
		{
			len = 1; // Longitud fija de 1 para los operadores simples
		}
		// Para cualquier otro token
		else
		{
			len = get_token_length(input, i);
		}

		new_tokens = malloc(sizeof(char *) * (token_count + 2));
		if (!new_tokens)
		{
			free_array(tokens);
			return (NULL);
		}

		int j;
		for (j = 0; j < token_count; j++)
			new_tokens[j] = tokens[j];

		new_tokens[token_count] = ft_substr(input, i, len);
		if (!new_tokens[token_count])
		{
			free(new_tokens);
			free_array(tokens);
			return (NULL);
		}

		new_tokens[token_count + 1] = NULL;
		token_count++;
		i += len;

		// Liberar el array de tokens anterior
		if (tokens)
			free(tokens);
		tokens = new_tokens;
	}

	return (tokens);
}
