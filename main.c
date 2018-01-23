// gcc main.c -L ./../../glfw/src -lglfw3 -framework OpenGL -framework Appkit -framework Cocoa -framework IOKit -framework CoreVideo && ./a.out

# define GLFW_INCLUDE_GLCOREARB

#include "../../glfw/include/GLFW/glfw3.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>

int	ft_strlen(char *s)
{
	int	a;

	a = -1;
	while (++a && *s && s[a])
		;
	return a;
}

typedef struct	s_env
{
	GLFWwindow* window;
	GLuint	vbo;
	GLuint	vao;
	GLuint	vs;
	GLuint	fs;
	GLuint	shader_programme;
	int		count;
}				t_env;

const char	*g_vertex_shader =
"#version 410\n"
"in vec3 uv;"
"vec3 u;"
"uniform float time;"
"void main()"
"{"
	"mat2 m = mat2(cos(time), sin(time), -sin(time), cos(time) );"
	"u = uv;"
	"u.xz *= m;"
	"gl_Position = vec4(u, 1.0);"
"}"
;

const char	*g_fragment_shader =
"#version 410\n"
"out vec4 frag_colour;"
"void main() {"
"  frag_colour = vec4(1.0);"
"}"
;

void	die(void)
{
	write(1, "!! die !!\n", 10);
	glfwTerminate();
}

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

int		mstrchr(char *s, int a)
{
	int	ret;

	ret = 0;
	while (s && *(s + ret) && *(s + ret) != (char)a )
		++ret;
	return (ret);
}

int		count_vertices(char *s, int *count)
{
	 float	i;
	 int	off;

	 off = 0;
	 while ( s && (3 == sscanf(s+off, "v %f %f %f\n", &i, &i, &i) ) )
	 {
		 (off += mstrchr(s+off, (int)'\n' )+1);
	 	 //printf("count vertices == %d, off == %d\n", *count, off);
		 ++(*count);
	 }
	 return (off);
}

void	count_faces(char *s, int *count)
{
	 int	i;
	 int	off;

	 off = 0;
	 while ( s && (3 == sscanf(s+off, "f %d %d %d\n", &i, &i, &i) ) )
	 {
		 (off += mstrchr(s+off, (int)'\n' )+1);
	 	 //printf("count faces == %d, off == %d\n", *count, off);
		 ++(*count);
	 }
}

void	assign_faces(char *s, t_faces *f, int	i)
{
	 int	off;
	 int	count;

	 off = 0;
	 count = 0;
	 while (count < i &&  s
			&& (f[count].indexes = (int *)malloc(sizeof(int)*3))
			&& (3 == sscanf(s+off, "f %d %d %d\n", &f[count].indexes[0], &f[count].indexes[1], &f[count].indexes[2]) )
		 )
	 {
		 off += mstrchr(s + off, (int)'\n' ) + 1;
	 	 printf("i == %d, count == %d, f[0] == %d\n", i, count, f[count].indexes[0]);
		 ++(count);
	 }
}

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

void	parse_file(char *obj_path, t_faces **faces, t_vertices **vertices, int *count_f)
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
	count_faces(s + off, &count);
	printf("s+off == |%s|\n", s+off);
	*faces = (t_faces *)malloc(sizeof(t_faces)*count);
	assign_faces(s + off, *faces, count);
	*count_f = count;
//	printf("f[0] == %d, f[0] == %d, f[2] == %d \n", *faces[count-1].indexes[0], *faces[count-1].indexes[1], *faces[count-1].indexes[2]);
}

static void	key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, 1);
}

void	render(t_env *e)
{
	static float	time = .0;
	GLint			time_loc;
	time_loc = glGetUniformLocation(e->shader_programme, "time");
	glProgramUniform1f(e->shader_programme, time_loc, time);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(e->shader_programme);
	glBindVertexArray(e->vao);
	//glBufferData(GL_ARRAY_BUFFER, e->count * 3 * sizeof(float), &faces[0], GL_STATIC_DRAW);
	//glDrawElements(GL_TRIANGLES, e->count*3, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_LINES, 0, e->count*3);
	glfwPollEvents();
	glfwSwapBuffers(e->window);
	time += .0046;
}

void	compile_shaders(t_env *e)
{
	e->vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(e->vs, 1, &g_vertex_shader, NULL);
	glCompileShader(e->vs);

	GLint isCompiled = 0;
	glGetShaderiv(e->vs, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
	GLint logSize = 0;
	GLint	maxLength = 1000;
	glGetShaderiv(e->vs, GL_INFO_LOG_LENGTH, &logSize);
	char *errorLog = (char *)malloc(sizeof(char)*logSize);
	glGetShaderInfoLog(e->vs, maxLength, &maxLength, &errorLog[0]);
	printf("could not compile shader\n");
	printf("%s\n", errorLog);
	}

	e->fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(e->fs, 1, &g_fragment_shader, NULL);
	glCompileShader(e->fs);

	e->shader_programme = glCreateProgram();
	glAttachShader(e->shader_programme, e->fs);
	glAttachShader(e->shader_programme, e->vs);
	glLinkProgram(e->shader_programme);
}

#define OBJ_PATH "teapot.obj"

void	init_scop(t_env *e)
{
	float	**points;
	int		fd;
	int		num_faces;

	fd = open(OBJ_PATH, O_RDONLY);
	if (!fd)
		die();

	t_faces		*faces_indexes;
	t_vertices	*vertices;
	faces_indexes = NULL;

	parse_file(OBJ_PATH, &faces_indexes, &vertices, &e->count);
	//printf("face_indexes = %p\n", faces_indexes);
	//printf("face index == %d\n", faces_indexes[0].indexes[0]);
	float	*faces = (float *)malloc(sizeof(float)*(e->count)*3*3);
	int		i = -1;
	printf("vertices[0].x == %f \n", vertices[0].point.x);
	float	scale = .1f;
	while(++i < e->count)
	{
		//printf("index == %d \n", faces_indexes[i].indexes[0]);
		*(t_vec3*)(faces + (i * 9 + 0)) = vertices[faces_indexes[i].indexes[0]-1].point;
		*(t_vec3*)(faces + (i * 9 + 3)) = vertices[faces_indexes[i].indexes[1]-1].point;
		*(t_vec3*)(faces + (i * 9 + 6)) = vertices[faces_indexes[i].indexes[2]-1].point;
		for (int j = 0; j < 9; j++)
			faces[i * 9 + j] *= scale;
//		faces[i*9+0] = vertices[faces_indexes[i].indexes[0]-1].point.x*scale;
		//	faces[i*9+1] = vertices[faces_indexes[i].indexes[0]-1].point.y*scale;
		//faces[i*9+2] = vertices[faces_indexes[i].indexes[0]-1].point.z*scale;

//		faces[i*9+3] = vertices[faces_indexes[i].indexes[1]-1].point.x*scale;
//		faces[i*9+4] = vertices[faces_indexes[i].indexes[1]-1].point.y*scale;
//		faces[i*9+5] = vertices[faces_indexes[i].indexes[1]-1].point.z*scale;

//		faces[i*9+6] = vertices[faces_indexes[i].indexes[2]-1].point.x*scale;
//		faces[i*9+7] = vertices[faces_indexes[i].indexes[2]-1].point.y*scale;
//		faces[i*9+8] = vertices[faces_indexes[i].indexes[2]-1].point.z*scale;
		//printf("%f\n", faces[i*3+0]);
	}

	glGenBuffers(1, &(e->vbo));
	glBindBuffer(GL_ARRAY_BUFFER, e->vbo);
	glBufferData(GL_ARRAY_BUFFER, 3*3*(e->count)*sizeof(float), &faces[0], GL_STATIC_DRAW);
	
	glGenVertexArrays(1, &(e->vao));
	glBindVertexArray(e->vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, e->vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	
	compile_shaders(e);
}

/*
**	To open a window on osx with glfw one MUST require a core profile
**	And set forward compatibiltity to true
*/

int		main(void)
{
	t_env	e;

	if (!glfwInit())
		write(1, "failed init !\n", 14);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	e.window = glfwCreateWindow(1920, 1440, "Scop", NULL, NULL);
	write(1, "test\n", 5);
	if (!e.window)
		die();
	glfwMakeContextCurrent(e.window);
	glfwSetKeyCallback(e.window, key_callback);
	init_scop(&e);
	while(!glfwWindowShouldClose(e.window) )
		render(&e);
	glfwDestroyWindow(e.window);
	die();
	return (0);
}
