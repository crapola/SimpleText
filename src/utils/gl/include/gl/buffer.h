#pragma once
#include "../../src/glid.hpp"
namespace gl
{
class Buffer: Identifiable
{
public:
	Buffer();
	~Buffer();
	using Identifiable::operator GLuint;
	void Bind(GLenum target) const
	{
		glBindBuffer(target,_id);
	}
private:
	using Identifiable::_id;
};
}
