// gcc main.c -L ./../../glfw/src -lglfw3 -framework OpenGL -framework Appkit -framework Cocoa -framework IOKit -framework CoreVideo && ./a.out

# define GLFW_INCLUDE_GLCOREARB

#include "../../glfw/include/GLFW/glfw3.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct	s_env
{
	GLFWwindow* window;
	GLuint	vbo;
	GLuint	vao;
	GLuint	vs;
	GLuint	fs;
	GLuint shader_programme;
}				t_env;

const char	*g_vertex_shader =
"#version 410\n"
"in vec3 uv;"
"void main()"
"{"
	"gl_Position = vec4(uv, 1.0);"
"}"
;

const char	*g_fragment_shader =
"#version 410\n"
"out vec4 frag_colour;"
"void main() {"
"  frag_colour = vec4(0.5, 0.0, 0.5, 1.0);"
"}"
;

void	die(void)
{
	write(1, "!! die !!\n", 10);
	glfwTerminate();
}

static void	key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, 1);
}

void	render(t_env *e)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(e->shader_programme);
	glBindVertexArray(e->vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glfwPollEvents();
	glfwSwapBuffers(e->window);
}

void	compile_shaders(t_env *e)
{
	e->vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(e->vs, 1, &g_vertex_shader, NULL);
	glCompileShader(e->vs);
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
	printf("before point\n");
	float faces[] = {-.0f, .50f, .0f, 
					.5f, -.5f, .0f,
					-.5f, -.50f, .0f
	};

	glGenBuffers(1, &(e->vbo));
	glBindBuffer(GL_ARRAY_BUFFER, e->vbo);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), faces, GL_STATIC_DRAW);
	
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
