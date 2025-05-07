/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:29:56 by mgrillo           #+#    #+#             */
/*   Updated: 2025/04/30 15:29:56 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Función: print_error_prefix
 * --------------------------
 * Imprime el prefijo de error de minishell.
 */
void	print_error_prefix(void)
{
	ft_putstr_fd(ERR_PREFIX, 2);
}

/*
 * Función: print_error_number
 * --------------------------
 * Imprime un número de error en la salida de error.
 *
 * Parámetros:
 *   number: Número a imprimir
 */
void	print_error_number(int number)
{
	char	*num;

	num = ft_itoa(number);
	ft_putstr_fd(num, 2);
	free(num);
}

/*
 * Función: handle_command_not_found
 * -------------------------------
 * Maneja el caso de comando no encontrado, actualizando el estado de salida
 * y mostrando el mensaje de error apropiado.
 *
 * Parámetros:
 *   cmd: Nombre del comando no encontrado
 *   shell: Estructura principal del shell
 */
void	handle_command_not_found(char *cmd, t_shell *shell)
{
	shell->exit_status = 127;
	if (ft_strncmp(cmd, "$?", 2) == 0)
		print_error_number(shell->exit_status);
	else
		ft_putstr_fd(cmd, 2);
	ft_putendl_fd(ERR_NOT_FOUND, 2);
}

/*
 * Función: handle_unclosed_quotes
 * -----------------------------
 * Maneja el caso de comillas sin cerrar, actualizando el estado de salida
 * y mostrando el mensaje de error apropiado.
 *
 * Parámetros:
 *   shell: Estructura principal del shell
 */
void	handle_unclosed_quotes(t_shell *shell)
{
	shell->exit_status = 2;
	print_error_prefix();
	ft_putendl_fd("Error: Unclosed quotes", 2);
}
