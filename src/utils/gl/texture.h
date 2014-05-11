#ifndef TEXTURE_H
#define TEXTURE_H
#include "glid.hpp"
namespace gl
{
class Texture: Identifiable
{
public:
	Texture();
	~Texture();
	using Identifiable::operator GLuint;
private:
	using Identifiable::_id;
};
}
#endif // TEXTURE_H
