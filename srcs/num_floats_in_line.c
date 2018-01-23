#include "../includes/Scop.h"

int		num_floats_in_line(char *line)
{
	int	ret;
	float tab[4];

	ret = sscanf(line, "f %d, %d, %d, %d\n", &tab[0], &tab[1], &tab[2], &tab[3]);

	return (ret);
}