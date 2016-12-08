#include <GL/glew.h>
#include <iostream>
namespace gl
{
void GLAPIENTRY CallBack(
	GLenum p_source,
	GLenum p_type,
	GLuint p_id,
	GLenum p_severity,
	GLsizei p_length,
	const GLchar *p_message,
	const void *p_userParam)
{
	std::cerr<<"OpenGL error: "<<p_message<<'\n';
}

void InitDebugProc()
{
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(CallBack,nullptr);
}
}
