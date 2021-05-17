#include "Shader.h"
#include"Maths.h"


Vec4f GouraudShader::vertex(int iface, int nthVert)
{
	return Vec4f();
}

bool GouraudShader::fragment(const Vec3f& bar, Color& color)
{
	return false;
}

GouraudShader::~GouraudShader()
{
}
