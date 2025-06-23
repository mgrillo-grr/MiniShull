/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:29:56 by mgrillo           #+#    #+#             */
/*   Updated: 2025/05/28 15:30:00 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <limits.h>

static int	update_pwd(t_shell *shell)
{
	char	old_pwd[PATH_MAX];
	char	*old_pwd_str;

	old_pwd_str = get_env_value(shell->env, "PWD");
	if (old_pwd_str)
		set_env_value(shell->env, "OLDPWD", old_pwd_str);
	if (getcwd(old_pwd, sizeof(old_pwd)))
		set_env_value(shell->env, "PWD", old_pwd);
	return (0);
}

static int	handle_cd_home(t_shell *shell, char **path)
{
	*path = get_env_value(shell->env, "HOME");
	if (!*path)
	{
		ft_putstr_fd(ERR_PREFIX"cd", 2);
		ft_putendl_fd(ERR_HOME, 2);
		return (1);
	}
	return (0);
}

static int	handle_cd_oldpwd(t_shell *shell, char **path)
{
	*path = get_env_value(shell->env, "OLDPWD");
	if (!*path)
	{
		ft_putstr_fd(ERR_PREFIX"cd", 2);
		ft_putendl_fd(ERR_OLDPWD, 2);
		return (1);
	}
	ft_putendl_fd(*path, 1);
	return (0);
}

int	cmd_cd(t_shell *shell, char **args)
{
	char	*path;
	int		should_free;
	char	*home;

	should_free = 0;
	if (args[1] && args[2])
	{
		ft_putstr_fd(ERR_PREFIX"cd: ", 2);
		ft_putendl_fd("too many arguments", 2);
		return (1);
	}

	if (args[1] && ft_strchr(args[1], '='))
	{
		ft_putstr_fd(ERR_PREFIX"cd: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putendl_fd(": not a valid identifier", 2);
		return (1);
	}

	if (!args[1] || ft_strncmp(args[1], "~", 1) == 0)
	{
		if (handle_cd_home(shell, &path))
			return (1);
		if (args[1] && args[1][1] == '/')
		{
			home = path;
			path = ft_strjoin(home, &args[1][1]);
			if (!path)
			{
				ft_putstr_fd(ERR_PREFIX"cd: ", 2);
				ft_putendl_fd(ERR_MEMORY, 2);
				return (1);
			}
			should_free = 1;
		}
	}
	else if (ft_strncmp(args[1], "-", 2) == 0)
	{
		if (handle_cd_oldpwd(shell, &path))
			return (1);
	}
	else
	{
		path = ft_strdup(args[1]);
		should_free = 1;
	}

	if (chdir(path) == -1)
	{
		ft_putstr_fd(ERR_PREFIX"cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putendl_fd(ERR_NO_FILE, 2);
		if (should_free)
			free(path);
		return (1);
	}
	update_pwd(shell);
	if (should_free)
		free(path);
	return (0);
}
