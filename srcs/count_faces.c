#include "../includes/Scop.h"

void	count_faces(char *s, int *count, int *num_vertexes)
{
	 int	i;
	 int	off;
	 int	nv;

	 off = 0;
	 nv = num_floats_in_line(s+off);
	 *num_vertexes += nv;
	 
	 while ( s && (nv == sscanf(s+off, "f %d %d %d %d\n", &i, &i, &i, &i) ) )
	 {
		 off += mstrchr(s+off, (int)'\n' ) + 1;
		 ++(*count);
		 if (*(s+off))
		 {
			 nv = num_floats_in_line(s+off);
			 *num_vertexes += nv;
		}
	 }
	 printf("%d == num\n", *num_vertexes);
}