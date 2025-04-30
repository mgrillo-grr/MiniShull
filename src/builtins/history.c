/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 20:16:00 by mgrillo           #+#    #+#             */
/*   Updated: 2025/04/30 20:16:00 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Función: cmd_history
 * -------------------
 * Muestra el historial de comandos.
 *
 * Parámetros:
 *   shell: Estructura principal del shell
 *   args: Argumentos del comando (no se utilizan)
 *
 * Retorna:
 *   0 en caso de éxito
 */
int	cmd_history(t_shell *shell, char **args)
{
	HIST_ENTRY	**history;
	int			i;

	(void)args;
	(void)shell;
	history = history_list();
	if (!history)
		return (0);

	i = 0;
	while (history[i])
	{
		ft_putnbr_fd(i + 1, 1);
		ft_putstr_fd("  ", 1);
		ft_putendl_fd(history[i]->line, 1);
		i++;
	}
	return (0);
}
