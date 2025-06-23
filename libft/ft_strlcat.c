/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 14:37:39 by mgrillo           #+#    #+#             */
/*   Updated: 2024/10/08 20:06:25 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	unsigned int	rsrc;
	unsigned int	lsrc;
	unsigned int	ldest;

	rsrc = 0;
	lsrc = 0;
	ldest = 0;
	while (dest[ldest] != '\0' && ldest < size)
		ldest++;
	while (src[lsrc] != '\0')
		lsrc++;
	if (size <= ldest)
		return (size + lsrc);
	while (src[rsrc] != '\0' && (ldest + rsrc + 1) < size)
	{
		dest[ldest + rsrc] = src[rsrc];
		rsrc++;
	}
	dest[ldest + rsrc] = '\0';
	return (ldest + lsrc);
}
