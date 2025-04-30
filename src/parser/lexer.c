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

	len = 0;
	while (str[start + len] && !is_special_char(str[start + len]) && 
		str[start + len] != ' ' && str[start + len] != '\t')
		len++;
	return (len);
}



/*
 * Función: tokenize
 * -----------------
 * Divide la entrada en tokens, separando por espacios y caracteres especiales.
 */
char	**tokenize(char *input)
{
	char	**tokens;
	char	**new_tokens;
	int		token_count;
	int		i;
	int		len;

	tokens = NULL;
	token_count = 0;
	i = 0;

	while (input[i])
	{
		skip_spaces(input, &i);
		if (!input[i])
			break;

		if (is_special_char(input[i]))
		{
			len = 1;
			if (input[i] == input[i + 1] && (input[i] == '<' || input[i] == '>'))
				len = 2;
			new_tokens = add_token(tokens, &token_count, input + i, len);
			i += len;
		}
		else
		{
			len = get_token_length(input, i);
			new_tokens = add_token(tokens, &token_count, input + i, len);
			i += len;
		}

		if (!new_tokens)
		{
			free_array(tokens);
			return (NULL);
		}
		if (tokens)
			free_array(tokens);
		tokens = new_tokens;
	}

	return (tokens);
}
