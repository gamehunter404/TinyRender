
#ifndef __ISHADER_H__
#define __ISHADER_H__

struct Vec4f;
struct Vec3f;
struct Color;

class IShader
{
public:
	virtual ~IShader() {}
	virtual Vec4f vertex(int iface,int nthVert) = 0;
	virtual bool fragment(const Vec3f& bar,Color& color) = 0;
};



class GouraudShader:public IShader
{
public:

	Vec4f vertex(int iface, int nthVert);
	bool fragment(const Vec3f& bar, Color& color);
	virtual ~GouraudShader();

};


#endif // !__ISHADER_H__

