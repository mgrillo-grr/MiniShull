/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 13:23:11 by mgrillo           #+#    #+#             */
/*   Updated: 2024/10/24 19:50:31 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_process_format(va_list args, const char *format)
{
	int	print_char;

	print_char = 0;
	if (*format == 'c')
		print_char += ft_putchar(va_arg(args, int));
	else if (*format == 's')
		print_char += ft_putstr(va_arg(args, char *));
	else if (*format == 'p')
		print_char += ft_put_ptr(va_arg(args, void *));
	else if (*format == 'd' || *format == 'i')
		print_char += ft_putnbr(va_arg(args, int));
	else if (*format == 'u')
		print_char += ft_put_unsigned(va_arg(args, unsigned int));
	else if (*format == 'x' || *format == 'X')
		print_char += ft_put_hex(va_arg(args, unsigned int), *format);
	else if (*format == '%')
		print_char += ft_putchar('%');
	return (print_char);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		print_char;

	print_char = 0;
	va_start(args, format);
	while (*format)
	{
		if (*format == '%')
		{
			format++;
			print_char += ft_process_format(args, format);
		}
		else
			print_char += write(1, format, 1);
		format++;
	}
	va_end(args);
	return (print_char);
}
