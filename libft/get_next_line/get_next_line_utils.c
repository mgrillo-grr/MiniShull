/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 19:17:24 by mgrillo           #+#    #+#             */
/*   Updated: 2025/03/05 21:01:30 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen_gnl(char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i] != '\0')
		i++;
	return (i);
}

char	*ft_strchr_gnl(char *save, int c)
{
	size_t	i;

	if (!save)
		return (0);
	i = 0;
	if (c == '\0')
		return (&save[ft_strlen_gnl(save)]);
	while (save[i] != '\0')
	{
		if (save[i] == (char) c)
			return (&save[i]);
		i++;
	}
	return (NULL);
}

char	*ft_strjoin_gnl(char *save, char *buffer)
{
	size_t		i;
	size_t		j;
	size_t		max;
	char		*str;

	if (!save)
	{
		save = (char *)malloc(1 * sizeof(char));
		save[0] = '\0';
	}
	if (!save || !buffer)
		return (NULL);
	max = ft_strlen_gnl(save) + ft_strlen_gnl(buffer);
	str = malloc(sizeof(char) * (max + 1));
	if (str == NULL)
		return (NULL);
	i = -1;
	j = 0;
	if (save)
		while (save[++i] != '\0')
			str[i] = save[i];
	while (buffer[j] != '\0')
		str[i++] = buffer[j++];
	str[ft_strlen_gnl(save) + ft_strlen_gnl(buffer)] = '\0';
	return (free(save), str);
}
