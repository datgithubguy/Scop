// gcc main.c -L ./../../glfw/src -lglfw3 -framework OpenGL -framework Appkit -framework Cocoa -framework IOKit -framework CoreVideo && ./a.out

#include "includes/Scop.h"

const char	*g_vertex_shader =
"#version 410\n"
"in vec3 uv;"
"out vec3 u;"
"uniform float time;"
"out float t;"
"void main()"
"{"
"t = time;"
	"mat2 m = mat2(cos(time), sin(time), -sin(time), cos(time) );"
	"u = uv;"
	"u.xz *= m;"
	"gl_Position = vec4(u, 1.0);"
"}"
;

const char	*g_fragment_shader =
"#version 410\n"
"out vec4 frag_colour;"
"in vec3 u;"
"in float t;"
"void main() {"
"  vec3 light = vec3(-1.+1.*sin(t*10.), 1., -1.+1.*cos(t*10.));"
"  float li = 1.-dot(light, normalize((u*3.)));"
"  li = pow(.625, abs(li));"
"  frag_colour = vec4(.5,.5,.5,1.0);"
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
	static float	time = .0;
	GLint			time_loc;
	time_loc = glGetUniformLocation(e->shader_programme, "time");
	glProgramUniform1f(e->shader_programme, time_loc, time);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(e->shader_programme);
	glBindVertexArray(e->vao);
	//glBufferData(GL_ARRAY_BUFFER, e->count * 3 * sizeof(float), &faces[0], GL_STATIC_DRAW);
	//glDrawElements(GL_TRIANGLES, e->count*3, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, e->count*3);
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
	isCompiled = 0;
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

	e->shader_programme = glCreateProgram();
	glAttachShader(e->shader_programme, e->fs);
	glAttachShader(e->shader_programme, e->vs);
	glLinkProgram(e->shader_programme);
}

#define OBJ_PATH "teapot3.obj"

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

	parse_file(OBJ_PATH, &faces_indexes, &vertices, &e->count, &e->num_vertexes);
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
