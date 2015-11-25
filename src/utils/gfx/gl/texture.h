#pragma once
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