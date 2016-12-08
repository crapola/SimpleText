#pragma once
#include "../../src/glid.hpp"
#include <string>

namespace gl
{
class Shader: Identifiable
{
public:
	Shader(GLenum p_type);
	~Shader();
	using Identifiable::operator GLuint;
	void Compile(const char* source) const;
	void Compile(const std::string& source) const;
private:
	using Identifiable::_id;
	static constexpr const char* _versionString="#version 420\n";
};
}
