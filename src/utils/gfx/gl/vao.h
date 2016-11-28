#pragma once
#include "glid.hpp"
namespace gl
{
class VAO : Identifiable
{
public:
	VAO();
	~VAO();
	using Identifiable::operator GLuint;
private:
	using Identifiable::_id;
};
}
