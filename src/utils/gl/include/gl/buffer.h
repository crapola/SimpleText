#pragma once
#include "../../src/glid.hpp"
namespace gl
{
class Buffer: public Identifiable
{
public:
	Buffer();
	~Buffer();
	void Bind(GLenum target) const;
};
}
