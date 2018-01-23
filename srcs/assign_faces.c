#include "../includes/Scop.h"

void	assign_faces(char *s, t_faces *f, int	i)
{
	 int	off;
	 int	count;

	 off = 0;
	 count = 0;
	 while (count < i &&  s
			&& (f[count].indexes = (int *)malloc(sizeof(int)*3))
			&& (3 == sscanf(s+off, "f %d %d %d\n", &f[count].indexes[0], &f[count].indexes[1], &f[count].indexes[2]) )
		 )
	 {
	 	f[count].count = 3;
		 off += mstrchr(s + off, (int)'\n' ) + 1;
		 ++(count);
	 }
}