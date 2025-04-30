/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:51:19 by mgrillo           #+#    #+#             */
/*   Updated: 2024/10/09 14:58:32 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	**ft_free(char **result, int i)
{
	while (i >= 0)
	{
		free(result[i]);
		i--;
	}
	free(result);
	return (NULL);
}

static int	count_words(const char *str, char c)
{
	int	count;
	int	word;

	count = 0;
	word = 0;
	while (str[count])
	{
		while (str[count] == c)
			count++;
		if (str[count] != '\0')
			word++;
		while (str[count] && str[count] != c)
			count++;
	}
	return (word);
}

static char	*word_dup(const char *s, int c)
{
	char	*word;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (s[i] && s[i] != c)
		i++;
	word = malloc((i + 1) * sizeof(char));
	if (!word)
		return (NULL);
	while (j < i)
	{
		word[j] = s[j];
		j++;
	}
	word[j] = '\0';
	return (word);
}

char	**ft_split(char const *s, char c)
{
	char	**result;
	int		i;
	int		words;

	i = 0;
	words = count_words(s, c);
	result = (char **)malloc((words + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	while (i < words)
	{
		while (*s == c)
			s++;
		result[i] = word_dup(s, c);
		if (!result[i])
			return (ft_free(result, i));
		while (*s && *s != c)
			s++;
		i++;
	}
	result[i] = NULL;
	return (result);
}
