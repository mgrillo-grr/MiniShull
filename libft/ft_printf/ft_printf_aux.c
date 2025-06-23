/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_aux.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 13:22:58 by mgrillo           #+#    #+#             */
/*   Updated: 2024/10/16 13:23:05 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putchar(char c)
{
	return (write(1, &c, 1));
}

int	ft_putstr(char *str)
{
	int	len;

	len = 0;
	if (!str)
		str = "(null)";
	while (*str)
		len += write(1, str++, 1);
	return (len);
}

int	ft_putnbr(int n)
{
	int		len;
	char	num[12];
	int		i;

	len = 0;
	i = 0;
	if (n == -2147483648)
		return (write(1, "-2147483648", 11));
	if (n < 0)
	{
		len += write(1, "-", 1);
		n = -n;
	}
	if (n == 0)
		return (write(1, "0", 1));
	while (n > 0)
	{
		num[i++] = n % 10 + '0';
		n /= 10;
	}
	while (i--)
		len += write(1, &num[i], 1);
	return (len);
}

int	ft_put_unsigned(unsigned int n)
{
	int		len;
	char	num[12];
	int		i;

	len = 0;
	i = 0;
	if (n == 0)
		return (write(1, "0", 1));
	while (n > 0)
	{
		num[i++] = n % 10 + '0';
		n /= 10;
	}
	while (i--)
		len += write(1, &num[i], 1);
	return (len);
}
