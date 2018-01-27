#include "../includes/Scop.h"

void	assign_faces(char *s, t_faces *f, int	i)
{
	 int	off;
	 int	count;
	 int	nv;

	 off = 0;
	 count = 0;
	 nv = num_floats_in_line(s+off);
	 while (count < i &&  s
			&& (f[count].indexes = (int *)malloc(sizeof(int)*nv))
			&& (nv == sscanf(s+off, "f %d %d %d %d\n", &f[count].indexes[0], &f[count].indexes[1], &f[count].indexes[2], &f[count].indexes[3]) )
		 )
	 {
	 	f[count].count = nv;
		off += mstrchr(s + off, (int)'\n' ) + 1;
		if (*(s+off))
			 nv = num_floats_in_line(s+off);
		++(count);
	 }
	 printf("assigned faces count == %d\n", count);
}