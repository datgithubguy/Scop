#include "../includes/Scop.h"

void	count_faces(char *s, int *count)
{
	 int	i;
	 int	off;

	 off = 0;
	 while ( s && (3 == sscanf(s+off, "f %d %d %d\n", &i, &i, &i) ) )
	 {
		 off += mstrchr(s+off, (int)'\n' ) + 1;
		 ++(*count);
	 }
}