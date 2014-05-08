#include "buffer.h"
#include <iostream>//
namespace gl
{

Buffer::Buffer():Identifiable(0)
{
	glGenBuffers(1,&_id);
	std::cout<<"Created buffer id="<<_id<<"\n";
}

Buffer::~Buffer()
{
	std::cout<<"Deleted buffer id="<<_id<<"\n";
	glDeleteBuffers(1,&_id);
}

}