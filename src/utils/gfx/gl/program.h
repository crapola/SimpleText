#pragma once
#include "glid.hpp"
namespace gl
{
class Program: Identifiable
{
public:
	Program();
	~Program();
	using Identifiable::operator GLuint;
	void Attach(GLuint shader) const;
	void Bind() const;
	void Link() const;
	static void Unbind();
private:
	using Identifiable::_id;
};
}