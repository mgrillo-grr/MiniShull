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
	while (str[start + len])
	{
		if (!quote && (str[start + len] == '\'' || str[start + len] == '"'))
			quote = str[start + len];
		else if (quote && str[start + len] == quote)
			quote = 0;
		else if (!quote && (is_special_char(str[start + len]) ||
			str[start + len] == ' ' || str[start + len] == '\t'))
			break;
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

		if (!is_special_char(input[i]) || 
			(input[i] == input[i + 1] && (input[i] == '<' || input[i] == '>')))
		{
			len = get_token_length(input, i);
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
		}
		else
		{
			len = 1;
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
		}

		if (tokens)
			free(tokens);
		tokens = new_tokens;
	}

	return (tokens);
}
