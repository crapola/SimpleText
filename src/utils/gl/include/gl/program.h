#pragma once
#include "../../src/glid.hpp"
namespace gl
{
class Program: public Identifiable
{
public:
	Program();
	~Program();
	void Attach(GLuint shader) const;
	void Bind() const;
	void Link() const;
	static void Unbind();
};
}
