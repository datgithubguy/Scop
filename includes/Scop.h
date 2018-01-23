#ifndef SCOP_H
 #define SCOP_H

 #define GLFW_INCLUDE_GLCOREARB
 #include "../../../glfw/include/GLFW/glfw3.h"
 #include <unistd.h>
 #include <fcntl.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <sys/stat.h>

typedef struct	s_env
{
	GLFWwindow* window;
	GLuint	vbo;
	GLuint	vao;
	GLuint	vs;
	GLuint	fs;
	GLuint	shader_programme;
	int		count;
	int		num_vertexes;
}				t_env;

typedef struct	s_vec3
{
	float	x;
	float	y;
	float	z;
}				t_vec3;

typedef struct	s_vertices
{
	t_vec3	point;
}				t_vertices;

typedef struct	s_faces
{
	int		*indexes;
	int		count;
}				t_faces;

void	die(void);
int		count_vertices(char *s, int *count);
void	count_faces(char *s, int *count, int *num_vertexes);
int		num_floats_in_line(char *line);
void	assign_faces(char *s, t_faces *f, int	i);
void	assign_vertices(char *s, t_vertices *v, int	i);
void	parse_file(char *obj_path, t_faces **faces, t_vertices **vertices,
					int *count_f, int *num_vertexes);
void	render(t_env *e);
void	compile_shaders(t_env *e);
void	init_scop(t_env *e);
int		mstrchr(char *s, int a);

#endif
