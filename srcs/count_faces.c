#include "../includes/Scop.h"

void	count_faces(char *s, int *count, int *num_vertexes)
{
	 int	i;
	 int	off;
	 int	nv;

	 off = 0;
	 nv = num_floats_in_line(s+off);
	 if (4 == nv)
	 	nv = 6;
	 *num_vertexes += nv;	 
	 while ( s && (nv = sscanf(s+off, "f %d %d %d %d\n", &i, &i, &i, &i) ) )
	 {
		off += mstrchr(s+off, (int)'\n' ) + 1;
		
		if (*(s+off) != '\0')
		{
		//	nv = num_floats_in_line(s+off);
			if (4 == nv)
			 	nv = 6;
			*num_vertexes += nv;
		}
		else
		{
			break;
		}
		++(*count);
		printf("%d == count\n", *count);
	 }
	 ++(*count);
	 printf("%d == num_vertexes\n", *num_vertexes);
}