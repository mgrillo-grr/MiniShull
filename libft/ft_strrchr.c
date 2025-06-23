/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 15:11:10 by mgrillo           #+#    #+#             */
/*   Updated: 2024/10/08 17:44:17 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *str, int c)
{
	const char	*last;

	last = 0;
	while (*str != '\0')
	{
		if (*str == (char)c)
		{
			last = str;
		}
		str++;
	}
	if ((char)c == '\0')
		return ((char *)str);
	return ((char *)last);
}
