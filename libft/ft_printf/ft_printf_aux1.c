/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_aux1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 13:23:08 by mgrillo           #+#    #+#             */
/*   Updated: 2024/10/24 19:54:04 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_put_hex(unsigned long num, char format)
{
	char	*hex_digits;
	char	buffer[16];
	int		i;
	int		print_char;

	if (format == 'x')
		hex_digits = "0123456789abcdef";
	else
		hex_digits = "0123456789ABCDEF";
	i = 0;
	print_char = 0;
	if (num == 0)
		return (write(1, "0", 1));
	while (num > 0)
	{
		buffer[i++] = hex_digits[num % 16];
		num /= 16;
	}
	while (i > 0)
		print_char += write(1, &buffer[--i], 1);
	return (print_char);
}

int	ft_put_ptr(void *ptr)
{
	int	len;

	len = 0;
	if (ptr == NULL)
	{
		len += write(1, "(nil)", 5);
		return (len);
	}
	len += write(1, "0x", 2);
	len += ft_put_hex((unsigned long )ptr, 'x');
	return (len);
}
