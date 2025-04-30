/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 12:25:08 by mgrillo           #+#    #+#             */
/*   Updated: 2024/09/25 12:25:08 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t num_byt)
{
	unsigned char		*dst;
	unsigned char		*source;

	if (!dest && !src)
		return (NULL);
	dst = ((unsigned char *)dest);
	source = ((unsigned char *)src);
	while (num_byt--)
	{
		((*dst++ = *source++));
	}
	return (dest);
}
