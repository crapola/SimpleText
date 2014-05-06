#ifndef CHECKGLERRORS_H_INCLUDED
#define CHECKGLERRORS_H_INCLUDED
// Writes tag, then last OpenGL error to standard output.
// Returns true if there was an error.
namespace gl
{
#ifndef NDEBUG
bool LogErrors(const char* tag);
#else
#define LogErrors(x)
#endif
}
#endif // CHECKGLERRORS_H_INCLUDED
