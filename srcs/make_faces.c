#include "../includes/Scop.h"

void	make_faces(float **faces, t_faces *faces_indexes, t_vertices *vertices, t_env *e)
{
	int		i;
	int		k;
	float	scale = .1f;

	i = -1;
	int	n = 0;
	while(++i < e->count+1)
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
			*(t_vec3*)( (*faces) + ( (i + 3 + n) * 3 * 3 )) = 
			vertices[faces_indexes[i].indexes[k] - 1].point;
			while (++k < 3)
			*(t_vec3*)( (*faces) + ( (i + 3 + n) * 3 * 3 + k * 3)) = 
			vertices[faces_indexes[i].indexes[k + 1] - 1].point;

			for (int j = 0; j < 3 * 3; j++)
				(*faces)[(i + 3 + n) * 3 * 3 + j] *= scale;
			n++;
		}
		/*
		printf("%d == faces_indexes[i].count, %d == i\n", faces_indexes[i].count, i );
		while (++k < faces_indexes[i].count)
			printf("AA\n"), *(t_vec3*)( (*faces) + (i * 3 * faces_indexes[i].count + 0 + k * faces_indexes[i].count ))
			= vertices[faces_indexes[i].indexes[k%faces_indexes[i].count]-1].point;
			//point = vertices[faces_indexes[i].indexes[k%faces_indexes[i].count]-1].point;
		for (int j = 0; j < 3 * faces_indexes[i].count; j++)
			printf("BB %d \n", i * 3 * faces_indexes[i].count ),(*faces)[i * 3 * faces_indexes[i].count + j] *= scale;
		*/
		printf("ya\n");
	}
	printf("argh\n");
}