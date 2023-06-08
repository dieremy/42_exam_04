#include "microshell.h"

int	print_err(char *str, char *av)
{
	while (*str)
		write(2, str++, 1);
	if (av)
		while (*av)
			write(2, av++, 1);
	write(2, "\n", 1);
	return (1);
}

int	exe(char *av[], int i, int tmp_fd, char *env[])
{
	av[i] = NULL;
	dup2(tmp_fd, 0);
	close(tmp_fd);
	execve(av[0], av, env);
	return (print_err("error: cannot execute ", av[0]));
}

int	exec_cmds(char *av[], int tmp_fd, char *env[])
{
	int	i;
	int	fd[2];

	i = 0;
	while (av[i] && av[i + 1])
	{
		av = &av[i + 1];
		i = 0;
		while (av[i] && strcmp(av[i], ";") && strcmp(av[i], "|"))
			i++;
		if (!strcmp(av[0], "cd"))
		{
			if (i != 2)
				print_err("error: cd: bad arguments", NULL);
			else if (chdir(av[1]))
				print_err("error: cd: cannot change directory to ", av[1]);
		}
		else if (i != 0 && (!av[i] || !strcmp(av[i], ";")))
		{
			if (!fork())
			{
				if (exe(av, i, tmp_fd, env))
					_exit(1);
			}
			else
			{
				close(tmp_fd);
				while (waitpid(-1, NULL, WUNTRACED) != -1)
					tmp_fd = dup(0);
			}
		}
		else if (i != 0 && !strcmp(av[i], "|"))
		{
			pipe(fd);
			if (!fork())
			{
				dup2(fd[1], 1);
				close(fd[0]);
				close(fd[1]);
				if (exe(av, i, tmp_fd, env))
					_exit(1);
			}
			else
			{
				close(fd[1]);
				close(tmp_fd);
				tmp_fd = fd[0];
			}
		}
	}
	close(tmp_fd);
	return (0);
}

int main(int ac, char *av[], char *env[])
{
	int	tmp_fd;

	(void)ac;
	tmp_fd = dup(0);
	exec_cmds(av, tmp_fd, env);
	return (0);
}
