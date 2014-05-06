#include "buffer.h"
namespace gl
{

Buffer::Buffer():Identifiable(0)
{
	glGenBuffers(1,&_id);
}

Buffer::~Buffer()
{
	glDeleteBuffers(1,&_id);
}

}