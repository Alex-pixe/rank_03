
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct s_figure {
	char r;
	float xtl;
	float ytl;
	float f_width;
	float f_hight;
	char draw;
} t_figure;

typedef struct s_fild {
	int width;
	int hight;
	char back;
	char *canvas;
	t_figure figure;
} t_fild;

int	pars_f(char *arg, t_fild *fild);
int	init_fild(t_fild *fild);
int	drawing_res(t_fild *fild);
int	put_rectangle(t_fild *fild);

int	main(int argc, char **argv)
{
	t_fild fild;
	if (argc != 2)
	{
		write(1, "Error: argument\n", 16);
		return (1);
	}
	if (pars_f(argv[1], &fild) == 1)
	{
		write(1, "Error: Operation file corrupted\n", 32);
		return (1);
	}
	return (drawing_res(&fild));
}

int	pars_f(char *arg, t_fild *fild)
{
	FILE *fd;
	int err;
	int i = 0;
	fd = fopen(arg, "r");
	if (!fd)
		return (1);
	if (fscanf(fd, "%d %d %c ", &fild->width, &fild->hight, &fild->back) != 3)
		return (1);
	if (!(fild->width > 0 && fild->width <= 300 && fild->hight > 0 && fild->hight <= 300))
		return (1);
	if (init_fild(fild))
		return (1);
	while ((err = fscanf(fd, "%c %f %f %f %f %c ", &fild->figure.r, &fild->figure.xtl, &fild->figure.ytl,\
	 &fild->figure.f_width, &fild->figure.f_hight, &fild->figure.draw)) == 6)
	{
		if (fild->figure.f_width <= 0 || fild->figure.f_hight <= 0)
		{
			free(fild->canvas);
			return (1);
		}
		if (put_rectangle(fild))
		{
			free(fild->canvas);
			return (1);
		}
		i++;
	}
	if (err == -1 && i >= 0)
		return (0);
	else
	{
		free(fild->canvas);
		return (1);
	}
}

int	put_rectangle(t_fild *fild)
{
	int i = 0;
	int j = 0;
	float xbr = fild->figure.xtl + fild->figure.f_width;
	float ybr = fild->figure.ytl + fild->figure.f_hight;
	if (fild->figure.r != 'r' && fild->figure.r != 'R')
		return (1);
	while (i < fild->hight)
	{
		j = 0;
		while (j < fild->width)
		{
			if (fild->figure.xtl <= j && xbr >= j && fild->figure.ytl <= i && ybr >= i)
			{
				if (fild->figure.r == 'R')
					fild->canvas[(fild->width + 1) * i + j] = fild->figure.draw;
				else
				{
					if ((ybr - i < 1 || i - fild->figure.ytl < 1) || (xbr - j < 1 || j - fild->figure.xtl < 1))
						fild->canvas[(fild->width + 1) * i + j] = fild->figure.draw;
				}
			}
			j++;
		}
		i++;
	}
	return (0);
}

int	init_fild(t_fild *fild)
{
	int i = 0;
	int j;
	fild->canvas = (char *)malloc(sizeof(char) * (fild->width + 1) * fild->hight);
	if (!fild->canvas)
		return (1);
	while (i < fild->hight)
	{
		j = 0;
		while (j < fild->width)
		{
			fild->canvas[(fild->width + 1) * i + j] = fild->back;
			j++;
		}
		fild->canvas[(fild->width + 1) * i + j] = '\n';
		i++;
	}
	return (0);
}

int	drawing_res(t_fild *fild)
{
	write(1, fild->canvas, (fild->width + 1) * fild->hight);
	free(fild->canvas);
	return (0);
}
