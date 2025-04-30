/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:29:56 by mgrillo           #+#    #+#             */
/*   Updated: 2025/04/30 15:29:56 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Variable global para manejar señales.
 * Se usa para comunicar entre el manejador de señales y el bucle principal.
 * Solo almacena el número de la señal recibida.
 */
int	g_signal_received = 0;

/*
 * Función: init_shell
 * ----------------
 * Inicializa la estructura principal del shell con los valores iniciales.
 *
 * Parámetros:
 *   shell: Puntero a la estructura principal del shell
 *   envp: Array de strings con las variables de entorno
 *
 * Proceso:
 * 1. Inicializa el entorno copiando las variables de envp
 * 2. Inicializa el comando actual como NULL
 * 3. Establece el estado de salida inicial como 0
 * 4. Marca que no estamos en un heredoc
 * 5. Configura los manejadores de señales
 */
static void	init_shell(t_shell *shell, char **envp)
{
	shell->env = init_env(envp);
	shell->cmd = NULL;
	shell->exit_status = 0;
	shell->in_heredoc = 0;
	shell->color_index = 0;
	setup_signals();
}

/*
 * Función: main
 * ------------
 * Función principal del shell. Implementa el bucle de lectura-evaluación-impresión (REPL).
 *
 * Parámetros:
 *   argc: Número de argumentos (no utilizado)
 *   argv: Array de argumentos (no utilizado)
 *   envp: Array de variables de entorno
 *
 * Proceso:
 * 1. Inicializa la estructura del shell
 * 2. Entra en un bucle infinito donde:
 *    - Lee una línea de entrada usando readline
 *    - Si la entrada es NULL (Ctrl+D), sale del bucle
 *    - Si la entrada no está vacía:
 *      a. Añade la línea al historial
 *      b. Parsea la entrada
 *      c. Si el parseo fue exitoso, ejecuta el comando
 *      d. Libera la memoria del comando
 *    - Libera la memoria de la línea de entrada
 * 3. Al salir, libera la memoria del entorno
 *
 * Retorna:
 *   El último estado de salida del shell
 */
/*
 * Función: get_prompt
 * -----------------
 * Genera el prompt con el formato usuario@host:directorio$ 
 *
 * Parámetros:
 *   shell: Estructura principal del shell
 *
 * Retorna:
 *   String con el prompt formateado
 */
static const char *g_colors[NUM_COLORS] = {
	COLOR_RED,
	COLOR_MAGENTA,
	COLOR_YELLOW,
	COLOR_GREEN,
	COLOR_BLUE,
	COLOR_CYAN
};

static char	*get_prompt(t_shell *shell)
{
	char	*user;
	char	hostname[HOST_NAME_MAX];
	char	cwd[MAX_PATH_LEN];
	char	*prompt;
	char	*home;
	char	*color;

	user = get_env_value(shell->env, "USER");
	if (!user)
		user = "user";
	if (gethostname(hostname, HOST_NAME_MAX) != 0)
		ft_strlcpy(hostname, "unknown", HOST_NAME_MAX);
	// Truncar el hostname en el primer punto
	char *dot = ft_strchr(hostname, '.');
	if (dot)
		*dot = '\0';
	if (!getcwd(cwd, MAX_PATH_LEN))
		ft_strlcpy(cwd, "~", MAX_PATH_LEN);
	home = get_env_value(shell->env, "HOME");
	if (home && ft_strncmp(cwd, home, ft_strlen(home)) == 0)
	{
		cwd[0] = '~';
		ft_memmove(cwd + 1, cwd + ft_strlen(home), 
			ft_strlen(cwd) - ft_strlen(home) + 1);
	}
	shell->color_index = (shell->color_index + 1) % NUM_COLORS;
	color = (char *)g_colors[shell->color_index];
	prompt = ft_calloc(MAX_PATH_LEN + HOST_NAME_MAX + 100, sizeof(char));
	if (!prompt)
		return (ft_strdup("$ "));
	sprintf(prompt, PROMPT, user, hostname, cwd, color);
	return (prompt);
}

/* Declaración de funciones externas */
extern void	handle_command_not_found(char *cmd, t_shell *shell);

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	char	*input;
	char	*prompt;
	char	*cmd_name;

	(void)argc;
	(void)argv;
	init_shell(&shell, envp);
	while (1)
	{
		prompt = get_prompt(&shell);
		input = readline(prompt);
		free(prompt);
		if (!input)
		{
			ft_putendl_fd("exit", 1);
			break;
		}
		if (*input)
		{
			add_history(input);
			shell.cmd = parse_input(input, &shell);
			if (shell.cmd && shell.cmd->args && shell.cmd->args[0])
			{
				cmd_name = ft_strdup(shell.cmd->args[0]);
				if (cmd_name)
				{
					shell.exit_status = execute_cmd(&shell);
					if (shell.exit_status == 127)
						handle_command_not_found(cmd_name, &shell);
				}
			}
			if (cmd_name)
				free(cmd_name);
			free_cmd(shell.cmd);
		}
		free(input);
	}
	free_env(shell.env);
	return (shell.exit_status);
}
