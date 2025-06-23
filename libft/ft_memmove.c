/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 12:25:15 by mgrillo           #+#    #+#             */
/*   Updated: 2024/09/25 12:25:15 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*dst;
	const unsigned char	*source;

	if (dest == src)
		return (dest);
	dst = (unsigned char *)dest;
	source = (const unsigned char *)src;
	if (dest < src)
		return (ft_memcpy(dest, src, n));
	dst += n;
	source += n;
	while (n--)
	{
		*(--dst) = *(--source);
	}
	return (dest);
}
