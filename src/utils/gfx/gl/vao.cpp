#include "vao.h"
#include <iostream>
namespace gl
{
VAO::VAO()
{
	glGenVertexArrays(1,&_id);
	std::cout<<"+VAO id="<<_id<<"\n";
}

VAO::~VAO()
{
	glDeleteVertexArrays(1,&_id);
	std::cout<<"-VAO id="<<_id<<"\n";
}
}
