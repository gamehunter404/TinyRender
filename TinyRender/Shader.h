
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

class FlatShader :public IShader
{
public:
	Vec4f vertex(int iface, int nthVert);
	bool fragment(const Vec3f& bar, Color& color);
	virtual ~FlatShader();

private:
	float getShadowFactor(const Vec3f& nor, const Vec3f& lightDir, const Vec3f& bar);

};

class TextureShader:public IShader
{
public:
	Vec4f vertex(int iface, int nthVert);
	bool fragment(const Vec3f& bar, Color& color);
	virtual ~TextureShader();
};

class LightIntensityShader :public IShader
{
public:
	virtual ~LightIntensityShader();
	Vec4f vertex(int iface, int nthVert);
	bool fragment(const Vec3f& bar, Color& color);
};

class NormalMapShader :public IShader
{
public:
	virtual ~NormalMapShader();
	Vec4f vertex(int iface, int nthVert);
	bool fragment(const Vec3f& bar, Color& color);
};

class PhongShader :public IShader
{
public:
	virtual ~PhongShader();
	Vec4f vertex(int iface, int nthVert);
	bool fragment(const Vec3f& bar, Color& color);

private:
	float k_d = 1.2f;
	float k_s = 0.6f;
	
};

class TangentNormalShader:public IShader
{
public :
	virtual ~TangentNormalShader();
	Vec4f vertex(int iface, int nthVert);
	bool fragment(const Vec3f& bar, Color& color);

private:
	Vec3f getTangentNormal(const Vec3f& uv,const Vec3f &bar);
};


class DepthShader :public IShader
{
public:
	virtual ~DepthShader();
	Vec4f vertex(int iface, int nthVert);
	bool fragment(const Vec3f& bar, Color& color);

};

class ShadowShader :public IShader
{
public:
	virtual ~ShadowShader();
	Vec4f vertex(int iface, int nthVert);
	bool fragment(const Vec3f& bar, Color& color);

private:
	float getShadowFactor(const Vec3f&nor,const Vec3f&lightDir,const Vec3f&bar);

private:
	float k_d = 1.2f;
	float k_s = 0.6f;
};

#endif // !__ISHADER_H__

