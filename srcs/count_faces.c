#include "../includes/Scop.h"

void	count_faces(char *s, int *count, int *num_vertexes)
{
	 int	i;
	 int	off;

	 off = 0;
	 *num_vertexes = num_floats_in_line(s+off);
	 printf("%d == num\n", *num_vertexes);
	 while ( s && (*num_vertexes == sscanf(s+off, "f %d %d %d %d\n", &i, &i, &i, &i) ) )
	 {
		 off += mstrchr(s+off, (int)'\n' ) + 1;
		 ++(*count);
	 }
}