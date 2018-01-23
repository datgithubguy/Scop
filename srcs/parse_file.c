#include "../includes/Scop.h"

void	parse_file(char *obj_path, t_faces **faces, t_vertices **vertices,
					int *count_f, int *num_vertexes)
{
	int		fd;
	char	*s;
	int		count;
	int		off = 0;

	count = 0;
	fd = open(obj_path, O_RDONLY);
	if (fd < 0)
		die();
	s = NULL;
	struct stat	stats;
	int			ret_stat;
	ret_stat = fstat(fd, &stats);
	s = (char *)malloc(sizeof(char)*(stats.st_size+1));
	read(fd, s, stats.st_size);
	s[stats.st_size] = '\0';
//	printf("|%s| == s\n", s);
	off = count_vertices(s, &count);
	*vertices = (t_vertices *)malloc(sizeof(t_vertices)*count);
	assign_vertices(s, *vertices, count);
	printf("v[0] == %f, v[0] == %f, v[2] == %f \n", (*vertices)[count-1].point.x, (*vertices)[count-1].point.y, (*vertices)[count-1].point.z);

	off += 1;
	count = 0;
	count_faces(s + off, &count, num_vertexes);
	//printf("s+off == |%s|\n", s+off);
	*faces = (t_faces *)malloc(sizeof(t_faces)*count);
	assign_faces(s + off, *faces, count);
	*count_f = count;
//	printf("f[0] == %d, f[0] == %d, f[2] == %d \n", *faces[count-1].indexes[0], *faces[count-1].indexes[1], *faces[count-1].indexes[2]);
}