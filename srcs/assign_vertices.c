#include "../includes/Scop.h"

void	assign_vertices(char *s, t_vertices *v, int	i)
{
	 int	off;
	 int	count;

	 off = 0;
	 count = 0;
	 while (count < i &&  s && (3 == sscanf(s+off, "v %f %f %f\n", &v[count].point.x, &v[count].point.y, &v[count].point.z) ) )
	 {
		 off += mstrchr(s + off, (int)'\n' ) + 1;
		 ++(count);
	 }
}