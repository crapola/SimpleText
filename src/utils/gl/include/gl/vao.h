#pragma once
#include "../../src/glid.hpp"
namespace gl
{
class VAO: Identifiable
{
public:
	VAO();
	~VAO();
	using Identifiable::operator GLuint;
	void Bind() const;
	static void Unbind();
private:
	using Identifiable::_id;
};
}
