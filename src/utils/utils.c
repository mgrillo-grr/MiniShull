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
 * Función: add_token
 * ------------------
 * Añade un nuevo token a la lista de tokens.
 */
char	**add_token(char **tokens, int *token_count, char *str, int len)
{
	char	**new_tokens;
	char	*new_str;
	int		i;

	new_str = ft_substr(str, 0, len);
	if (!new_str)
		return (NULL);

	new_tokens = malloc(sizeof(char *) * (*token_count + 2));
	if (!new_tokens)
	{
		free(new_str);
		return (NULL);
	}

	i = 0;
	while (i < *token_count)
	{
		new_tokens[i] = ft_strdup(tokens[i]);
		if (!new_tokens[i])
		{
			free(new_str);
			while (--i >= 0)
				free(new_tokens[i]);
			free(new_tokens);
			return (NULL);
		}
		i++;
	}

	new_tokens[*token_count] = new_str;
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
