#include "draw.h"


void init()
{
	GLuint vertex_shader;
	GLuint fragment_shader;

	GLchar *vertex_shader_source;
	GLchar *fragment_shader_source;

	LOADSHADERFROMFILE(vertex_shader_source, "vertex.glsl");
	LOADSHADERFROMFILE(fragment_shader_source, "fragment.glsl");

	LOADOPENGLPROC(PFNGLSHADERSOURCEARBPROC, glShaderSource);
	LOADOPENGLPROC(PFNGLCREATESHADERPROC, glCreateShader);
	LOADOPENGLPROC(PFNGLCOMPILESHADERPROC, glCompileShader);
	LOADOPENGLPROC(PFNGLATTACHSHADERPROC, glAttachShader);
	LOADOPENGLPROC(PFNGLLINKPROGRAMPROC, glLinkProgram);
	LOADOPENGLPROC(PFNGLDELETESHADERPROC, glDeleteShader);
	LOADOPENGLPROC(PFNGLCREATEPROGRAMPROC, glCreateProgram);
	LOADOPENGLPROC(PFNGLGENVERTEXARRAYSPROC, glGenVertexArrays);
	LOADOPENGLPROC(PFNGLBINDVERTEXARRAYAPPLEPROC, glBindVertexArray);
	LOADOPENGLPROC(PFNGLCLEARBUFFERFVPROC, glClearBufferfv);
	LOADOPENGLPROC(PFNGLUSEPROGRAMPROC, glUseProgram);
	LOADOPENGLPROC(PFNGLDELETEVERTEXARRAYSPROC, glDeleteVertexArrays);
	LOADOPENGLPROC(PFNGLDELETEPROGRAMPROC, glDeleteProgram);
	LOADOPENGLPROC(PFNGLVERTEXATTRIB4FVPROC, glVertexAttrib4fv);

	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
	glCompileShader(vertex_shader);
	free(vertex_shader_source);

	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
	glCompileShader(fragment_shader);
	free(fragment_shader_source);

	rendering_program = glCreateProgram();
	glAttachShader(rendering_program, vertex_shader);
	glAttachShader(rendering_program, fragment_shader);
	glLinkProgram(rendering_program);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	glGenVertexArrays(3, &vertex_array_object);
	glBindVertexArray(vertex_array_object);
}

void update()
{
	GLfloat color[] = { 
		(float)sin((double)get_msec_time()) * 0.5f + 0.5f, 
		(float)cos((double)get_msec_time()) * 0.5f + 0.5f, 
		0.0f, 
		1.0f 
	};

	GLfloat attrib[] = {
		(float)sin((double)get_msec_time()) * 0.5f, 
		(float)cos((double)get_msec_time()) * 0.6f, 
		0.0f, 
		0.0f 
	};

	glClearBufferfv(GL_COLOR, 0, color);
	glUseProgram(rendering_program);
	glVertexAttrib4fv(0, attrib);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	swap_buffers();
}

void cleanup()
{
	glDeleteVertexArrays(1, &vertex_array_object);
	glDeleteProgram(rendering_program);
	glDeleteVertexArrays(1, &vertex_array_object);
}

GLchar *load_shader_code(char *filename)
{
	FILE *file;
	long int size;
	GLchar* result;

	if (!(file = fopen(filename, "rb"))) return NULL;

	fseek(file, 0, SEEK_END);
	size = ftell(file);
	fseek(file, 0, SEEK_SET);

	if (!(result = (GLchar *)malloc(size / sizeof(GLchar))))
	{
		fclose(file);
		return NULL;
	}

	fread(result, 1, size, file);

	fclose(file);

	return result;
}


double get_msec_time()
{
	SYSTEMTIME systemtime;
	
	GetSystemTime(&systemtime);

	return ((double)time(NULL)) + ((double)systemtime.wMilliseconds / 1000.0f);
}
