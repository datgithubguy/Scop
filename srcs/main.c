//gcc srcs/*.c -L ./../../glfw/src -lglfw3 -framework OpenGL -framework Appkit -framework Cocoa -framework IOKit -framework CoreVideo -Wall -Wextra -Werror -fsanitize=address -g && ./a.out

#include "../includes/Scop.h"

const char	*g_vertex_shader =
"#version 410\n"
"in vec3 uv;"
"out vec3 u;"
"in vec2 ttx;"
"out vec2 tx;"
"uniform float time;"
//"inout vec2 tx;"
"void main()"
"{"
	"mat2 m = mat2(cos(time), sin(time), -sin(time), cos(time) );"
	"u = uv;"
//	"u.xz *= m;"
	//"u.yz *= m;"
	"tx = ttx;"
	"gl_Position = vec4(u, 1.0);"
"}"
;

const char	*g_fragment_shader =
"#version 410\n"
"uniform sampler2D myTextureSampler;"
"out vec4 frag_colour;"
"in vec3 u;"
"in vec3 normal;"
"uniform float time;"
"in vec2 tx;"
"void main() {"
"  vec3 light = vec3(+cos(time*4.)*0.+2., +sin(time*4.)*0.+2., +sin(time*4.)*20.)*0.+1.1;"
"  float li = .5*clamp(dot(normal, (light) ), .0, 1.);"
"  li = 1.-pow(.125, abs(li));"
//"	mat2 m = mat2(cos(time*-1.), sin(time*-1.), -sin(time*-1.), cos(time*-1.) );"
//"	vec3 ux = u;"
//"	ux.xz *= m;"
//"	vec3 nx = normal;"
//"	nx.xz *= m;"
//"	li += .52*(1.0 - max(dot(normalize(-ux), nx), .0));"
//"	li = u.x;"
"  frag_colour = 1.0*vec4( li*.9+.2,li*.75+.2, -li+.71, 1.0);"
//"  frag_colour += vec4( texture(myTextureSampler, tx.xy*5.).xyz, 1.0);"
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
"out vec3 u;"
"out vec2 tx;"
"void	main()"
"{"
"float aspect = 1920./1440.;"
"float aa = 1./(tan(1.58/2.));"
"float near = .50;"
"float far  = 100.01;"
// "mat4 perspective = mat4("
// 				"aa/aspect, .0, .0, .0, "
// 				".0, aa, .0, .0,"
// 				".0, .0, -((far+near)/(far - near)), -1.,"
// 				".0, .0, -((2. * far * near) / (far - near)), .0"
// ");"
"mat4 perspective = mat4("
				"aa/aspect, .0, .0, .0, "
				".0, aa, .0, .0,"
				".0, .0, -((far+near)/(far - near)), -1.,"
				".0, .0, -((2. * far * near) / (far - near)), .0"
");"
"float W = aspect*atan(2./2.);"
"float H = W;"
"float Q = far/(far-near);"
"perspective = mat4("
				"1.0, .0, .0, .0, "
				".0, 1.0, .0, .0,"
				".0, .0, 1.0, .0,"
				".0, .0, .0, 1.0"
");"

"perspective = mat4("
				"W, .0, .0, .0, "
				".0, H, .0, .0,"
				".0, .0, -(far+near)/(far-near), -2.*(near*far)/(far-near),"
				".0, .0, -1., .0"
");"

"mat4 world_proj = mat4("
				"1./1., .0, .0, .0, "
				".0, 1./1., .0, .0,"
				".0, .0, -2./(far-near), -(far+near)/(far-near),"
				".0, .0, .0, 1.0"
");"

"float id = float(gl_PrimitiveIDIn);"
"mat2 m = mat2(cos(time*1.), sin(time*1.), -sin(time*1.), cos(time*1.) );"
//"	if (mod(id+floor(time*8.), 5.) >= +floor(time*1.) )"
//"	if (id < floor(time*1000.))"
"	for (int i = 0; i < 3; i++)"
	"{"
		"gl_out.gl_Position = "
		"gl_in[i].gl_Position-vec4(-.0,.0,.0, .0);"
		"u = gl_out.gl_Position.xyz;"
		"tx = vec2( (max(u.x, u.z)), u.y);"
		//"tx = vec2( min(min(u.xy, u.xz), u.zy));"
		//"gl_out.gl_Position.xy /= gl_out.gl_Position.z+1.;"
		//"gl_out.gl_Position.xz *= m;"
	"vec4 test_pos = vec4(gl_out.gl_Position.xyz, 1.);"
//		"gl_out.gl_Position.xyzw *= perspective;"
//"test_pos.w = gl_out.gl_Position.w;"
"test_pos.xz *= m;"
"test_pos.zy *= m;"
"test_pos.xyzw *= world_proj;"
"test_pos.xyzw *= perspective;"
//"test_pos /= test_pos.w;"
//"test_pos.xz *= m;"
		//"u = gl_out.gl_Position.xyz/gl_out.gl_Position.z;"
		"u = gl_out.gl_Position.xyz;"
"u = test_pos.xyz;"
//"u.xz *=m;"
"gl_out.gl_Position.xyz = u;"
		"vec3 tmp_0, tmp_1, tmp_2;"
		"tmp_0 = gl_in[2].gl_Position.xyz;"
		"tmp_1 = gl_in[1].gl_Position.xyz;"
		"tmp_2 = gl_in[0].gl_Position.xyz;"
		"tmp_0.xz *= m;"
		"tmp_1.xz *= m;"
		"tmp_2.xz *= m;"
		"vec3 A = tmp_0 - tmp_2;"
	    "vec3 B = tmp_1 - tmp_2;"
	    "normal = -normalize(cross(A,B));"
	    //"gl_out.gl_Position.xyz += normal*-.01f*abs(sin(time*8.));"
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

//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glProgramUniform1f(e->shader_programme, time_loc, time);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(e->shader_programme);
	glBindVertexArray(e->vao);
	//glBufferData(GL_ARRAY_BUFFER, e->count * 3 * sizeof(float), &faces[0], GL_STATIC_DRAW);
	//glDrawElements(GL_TRIANGLES, e->count*3, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, e->num_vertexes*3*sizeof(float) );
	//glDrawArrays(GL_LINES, 0, e->num_vertexes*2*sizeof(float));
	glfwPollEvents();
	glfwSwapBuffers(e->window);
	time += .0046;
}

void	create_texture(t_env *e)
{
	(void)e;
	GLuint	texture_id;
	char	*data;
	int		width;
	int		height;

	data = get_bmp_img(&width, &height);

	printf("%s == img\n", data);
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void	compile_shaders(t_env *e)
{
	create_texture(e);
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
		die();
	}

	e->shader_programme = glCreateProgram();
	glAttachShader(e->shader_programme, e->vs);
	glAttachShader(e->shader_programme, e->gs);
	glAttachShader(e->shader_programme, e->fs);
	glLinkProgram(e->shader_programme);
}

#define OBJ_PATH "teapot.obj"

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
	faces = (float *)malloc(e->num_vertexes * 3 * sizeof(float));
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
