#ifndef GLID_HPP_INCLUDED
#define GLID_HPP_INCLUDED
#include <GL/glew.h>
namespace gl
{
class Identifiable
{
protected:
	Identifiable():_id(0){}
	explicit Identifiable(const GLuint id):_id(id){}
	Identifiable(const Identifiable&)=delete;
	Identifiable& operator=(const Identifiable&)=delete;
	~Identifiable(){}
	operator GLuint() const
	{
		return _id;
	}
	GLuint _id;
};
}
#endif // GLID_HPP_INCLUDED