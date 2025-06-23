/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:42:34 by mgrillo           #+#    #+#             */
/*   Updated: 2025/05/27 15:55:25 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_UTILS_H
# define BUILTIN_UTILS_H

# include "minishell.h"

// Prototipos de las funciones de los comandos built-in
int	cmd_echo(t_shell *shell, char **args);
int	cmd_cd(t_shell *shell, char **args);
int	cmd_pwd(t_shell *shell, char **args);
int	cmd_export(t_shell *shell, char **args);
int	cmd_unset(t_shell *shell, char **args);
int	cmd_env(t_shell *shell, char **args);
int	cmd_exit(t_shell *shell, char **args);
int	cmd_history(t_shell *shell, char **args);

typedef struct s_builtin {
	char	*name;
	int		(*func)(t_shell *, char **);
} t_builtin;

// Obtener el array de comandos built-in
const t_builtin	*get_builtins(void);

// Obtener el número de comandos built-in definidos
size_t	get_builtins_count(void);

#endif
