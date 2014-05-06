#ifndef SHADER_H
#define SHADER_H
#include "glid.hpp"
namespace gl
{
class Shader: Identifiable
{
public:
	Shader(GLenum p_type);
	~Shader();
	using Identifiable::operator GLuint;
	void Compile(const char* source) const;
private:
	using Identifiable::_id;
	static constexpr const char* _versionString="#version 420\n";
};
}
#endif // SHADER_H
