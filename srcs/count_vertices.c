#include "../includes/Scop.h"

int		count_vertices(char *s, int *count)
{
	 float	i;
	 int	off;

	 off = 0;
	 while ( s && (3 == sscanf(s+off, "v %f %f %f\n", &i, &i, &i) ) )
	 {
		 (off += mstrchr(s+off, (int)'\n' )+1);
		 ++(*count);
	 }
	 return (off);
}