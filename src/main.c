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
	shell->env = init_env(envp); /*Inicializa el entorno del shell llamando a una función init_env() que probablemente convierte envp en una estructura propia del proyecto (como una lista o diccionario).*/
	shell->cmd = NULL; /* Inicializa el puntero a comandos a NULL, lo que indica que aún no hay ningún comando cargado. */
	shell->exit_status = 0; /* Inicializa el código de salida del último comando ejecutado a 0 (éxito). */
	shell->in_heredoc = 0; /* Probablemente indica si el shell está actualmente dentro de un heredoc (una forma de redirección de entrada), inicializándolo a false o 0. */
	shell->color_index = 0; /* Esto parece estar relacionado con colores en la terminal (por ejemplo, cambiar el color del prompt), inicializándolo al primer color. */
	setup_signals(); /* Llama a una función para configurar las señales del sistema (como SIGINT, SIGQUIT, etc.), necesarias para manejar interrupciones del usuario. */
}


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
			cmd_name = NULL;
			add_history(input);
			shell.cmd = parse_input(input, &shell);
			if (!shell.cmd && *input)
			{
				handle_unclosed_quotes(&shell);
			}
			else if (shell.cmd && shell.cmd->args && shell.cmd->args[0])
			{
				cmd_name = ft_strdup(shell.cmd->args[0]);
				if (cmd_name)
				{
					shell.exit_status = execute_cmd(&shell);
					free(cmd_name);
				}
			}
			if (shell.cmd)
				free_cmd(shell.cmd);
		}
		free(input);
	}
	free_env(shell.env);
	return (shell.exit_status);
}
