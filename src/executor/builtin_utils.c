/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:42:34 by mgrillo           #+#    #+#             */
/*   Updated: 2025/05/27 15:55:25 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_utils.h"

// Función estática que devuelve el array de comandos built-in
static const t_builtin	*get_builtins_array(void)
{
	static const t_builtin	builtins[] = {
	{"echo", &cmd_echo},
	{"cd", &cmd_cd},
	{"pwd", &cmd_pwd},
	{"export", &cmd_export},
	{"unset", &cmd_unset},
	{"env", &cmd_env},
	{"exit", &cmd_exit},
	{"history", &cmd_history},
	{NULL, NULL}
	};

	return (builtins);
}

// Obtener el array de comandos built-in
const t_builtin	*get_builtins(void)
{
	return (get_builtins_array());
}

// Obtener el número de comandos built-in definidos
size_t	get_builtins_count(void)
{
	const t_builtin	*builtins;
	size_t			count;

	builtins = get_builtins();
	count = 0;
	while (builtins[count].name != NULL)
		count++;
	return (count);
}
