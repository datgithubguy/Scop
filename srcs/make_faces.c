#include "../includes/Scop.h"

void	make_faces(float **faces, t_faces *faces_indexes, t_vertices *vertices, t_env *e)
{
	int		i;
	int		k;
	float	scale = .251f;

	i = -1;
	int	n = 0;
	while(++i < e->count)
	{
		printf("e count == %d, i == %d \n", e->count, i);
		k = -1;
		while (++k < 3)
		*(t_vec3*)( (*faces) + ((i + n) * 3 * 3 + k * 3 )) = 
		vertices[faces_indexes[i].indexes[k]-1].point;
		for (int j = 0; j < 3 * 3; j++)
			(*faces)[(i + n) * 3 * 3 + j] *= scale;
		if (faces_indexes[i].count == 4)
		{
			k = 0;
			*(t_vec3*)( (*faces) + ( (i + 1 + n) * 3 * 3 )) = 
			vertices[faces_indexes[i].indexes[k] - 1].point;
			while (++k < 3)
			*(t_vec3*)( (*faces) + ( (i + 1 + n) * 3 * 3 + k * 3)) = 
			vertices[faces_indexes[i].indexes[k + 1] - 1].point;

			for (int j = 0; j < 3 * 3; j++)
				(*faces)[(i + 1 + n) * 3 * 3 + j] *= scale;
			n++;
		}
		printf("ya\n");
	}
	printf("argh\n");
}