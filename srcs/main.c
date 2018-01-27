//gcc srcs/*.c -L ./../../glfw/src -lglfw3 -framework OpenGL -framework Appkit -framework Cocoa -framework IOKit -framework CoreVideo -Wall -Wextra -Werror -fsanitize=address -g && ./a.out

#include "../includes/Scop.h"

const char	*g_vertex_shader =
"#version 410\n"
"in vec3 uv;"
"out vec3 u;"
"uniform float time;"
"out float t;"
//"inout vec2 tx;"
"void main()"
"{"
"t = time;"
	"mat2 m = mat2(cos(time), sin(time), -sin(time), cos(time) );"
	"u = uv;"
//	"u.xz *= m;"
	"u.yz *= m;"
	"gl_Position = vec4(u, 1.0);"
"}"
;

const char	*g_fragment_shader =
"#version 410\n"
"out vec4 frag_colour;"
"in vec3 u;"
"in vec3 normal;"
"in float t;"
//"in vec2 tx;"
"void main() {"
"  vec3 light = vec3(-1., 1.0, 1.)*.1;"
//"	"
"  float li = .5*clamp(1.-dot(normal, (light) ), .0, 1.);"
//"  li = pow(.625, abs(li));"
"  frag_colour = vec4( li*.9+.2,li*.75+.2, 1., 1.0);"
"}"
;

const char	*g_geometry_shader =
"#version 410\n"
"layout(triangles) in;"
"layout(triangle_strip, max_vertices = 3) out;"
"uniform float time;"
"in gl_PerVertex"
"{"
  "vec4 gl_Position;"
"} gl_in[];"
"out gl_PerVertex"
"{"
  "vec4 gl_Position;"
"} gl_out;"
"out vec3 normal;"
"void	main()"
"{"
"float id = 6.28*float(gl_PrimitiveIDIn)/45.;"
"mat2 m = mat2(cos(time*1.1), sin(time*1.1), -sin(time*1.1), cos(time*1.1) );"
"	for(int i = 0; i < 3; i++)"
	"{"
		"gl_out.gl_Position = "
		"gl_in[i].gl_Position-vec4(-.0,.0,.0, .0);"
		"gl_out.gl_Position.xz *= m;"
		"vec3 tmp_0, tmp_1, tmp_2;"
		"tmp_0 = gl_in[2].gl_Position.xyz;"
		"tmp_1 = gl_in[1].gl_Position.xyz;"
		"tmp_2 = gl_in[0].gl_Position.xyz;"
		"tmp_0.xz *= m;"
		"tmp_1.xz *= m;"
		"tmp_2.xz *= m;"
		"vec3 A = tmp_0 - tmp_2;"
	    "vec3 B = tmp_1 - tmp_2;"
	    "normal = normalize(cross(A,B));"
		"EmitVertex();"
	"}"
	"EndPrimitive();"
"}"
;

void	die(void)
{
	write(1, "!! die !!\n", 10);
	glfwTerminate();
}

static void	key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	(void)scancode;
	(void)mods;
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
	glDrawArrays(GL_TRIANGLES, 0, e->num_vertexes*3*sizeof(float) );
	// glDrawArrays(GL_POINTS, 0, e->num_vertexes*3*sizeof(float));
	glfwPollEvents();
	glfwSwapBuffers(e->window);
	time += .05046;
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
		printf("could not compile vertex shader\n");
		printf("%s\n", errorLog);
	}

	e->gs = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(e->gs, 1, &g_geometry_shader, NULL);
	glCompileShader(e->gs);

	isCompiled = 0;
	glGetShaderiv(e->gs, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint logSize = 100;
		GLint	maxLength = 1000;
		glGetShaderiv(e->gs, GL_INFO_LOG_LENGTH, &logSize);
		char *errorLog = (char *)malloc(sizeof(char)*logSize);
		glGetShaderInfoLog(e->gs, maxLength, &maxLength, &errorLog[0]);
		printf("could not compile geometry shader\n");
		printf("%s\n", errorLog);
	}

	e->fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(e->fs, 1, &g_fragment_shader, NULL);
	glCompileShader(e->fs);
	isCompiled = 0;
	glGetShaderiv(e->fs, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint logSize = 100;
		GLint	maxLength = 1000;
		glGetShaderiv(e->fs, GL_INFO_LOG_LENGTH, &logSize);
		char *errorLog = (char *)malloc(sizeof(char)*logSize);
		glGetShaderInfoLog(e->fs, maxLength, &maxLength, &errorLog[0]);
		printf("could not compile fragment shader\n");
		printf("%s\n", errorLog);
	}

	e->shader_programme = glCreateProgram();
	glAttachShader(e->shader_programme, e->vs);
	glAttachShader(e->shader_programme, e->gs);
	glAttachShader(e->shader_programme, e->fs);
	glLinkProgram(e->shader_programme);
}

#define OBJ_PATH "42b.obj"

void	init_scop(t_env *e)
{
	float	*faces;
	int		fd;

	fd = open(OBJ_PATH, O_RDONLY);
	if (!fd)
		die();

	t_faces		*faces_indexes;
	t_vertices	*vertices;
	faces_indexes = NULL;

	parse_file(OBJ_PATH, &faces_indexes, &vertices, &e->count, &e->num_vertexes);
	faces = (float *)malloc(sizeof(float) * 3 * e->num_vertexes);
	bzero(faces, e->num_vertexes * 3 * sizeof(float));
	make_faces(&faces, faces_indexes, vertices, e);

	glGenBuffers(1, &(e->vbo));
	glBindBuffer(GL_ARRAY_BUFFER, e->vbo);
	glBufferData(GL_ARRAY_BUFFER, e->num_vertexes * 3 * sizeof(float), &faces[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &(e->vao));
	glBindVertexArray(e->vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, e->vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	compile_shaders(e);
	glEnable(GL_DEPTH_TEST);
}

/*
**	To open a window on osx with glfw one MUST require a core profile
**	And set forward compatibiltity to true
*/

int		main(void)
{
	t_env	e;

	bzero(&e, sizeof(t_env)); // highly important
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
