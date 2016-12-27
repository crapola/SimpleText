#pragma once
#include "../../src/glid.hpp"
namespace gl
{
class VAO: public Identifiable
{
public:
	VAO();
	~VAO();
	void Bind() const;
	static void Unbind();
};
}
