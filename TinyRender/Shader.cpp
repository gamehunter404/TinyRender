#include "Shader.h"
#include"Maths.h"
#include"Device.h"
#include<iostream>

Vec4f TextureShader::vertex(int iface, int nthVert)
{
	ObjModel& model = *(gl_Device.model);
	Vec4f v = vec4f_SetPoint(model.vert(iface, nthVert));
	v = mat4x4_Mul_Vec4f(gl_Device.modelMat,v);// to world space
	v = mat4x4_Mul_Vec4f(gl_Device.uniform_projectionMat,v); // to camera space, and then to cvv
	v = mat4x4_Mul_Vec4f(gl_Device.viewPortMat,v); //to window space

	gl_Device.varying_uvs[nthVert] = model.uvs(iface,nthVert);
	projVec(mat4x4_Mul_Vec4f(gl_Device.inverseModelMat,
		vec4f_SetVec(model.vns(iface, nthVert))),&gl_Device.varying_vns[nthVert]);

	return Vec4f(v.x/v.w,v.y/v.w,v.z/v.w,v.w);
}
bool TextureShader::fragment(const Vec3f& bar, Color& color)
{
	Vec3f nor = vec3f_Add(vec3f_Mul(gl_Device.varying_vns[0],bar.x),
						  vec3f_Mul(gl_Device.varying_vns[1], bar.y), 
						  vec3f_Mul(gl_Device.varying_vns[2], bar.z));

	//nor = vec3f_baryInterpolation(gl_Device.);

	Vec3f uv = vec3f_Add(vec3f_Mul(gl_Device.varying_uvs[0],bar.x),
						 vec3f_Mul(gl_Device.varying_uvs[1],bar.y),
						 vec3f_Mul(gl_Device.varying_uvs[2],bar.z));

	nor.normalize();
	float intensity = std::min(vec3f_Dot(-nor, gl_Device.light_Dir),1.0f);
	color = gl_Device.texture->getColor(uv.x,(1.-uv.y))*intensity;

	return false;
}
TextureShader::~TextureShader()
{
}


LightIntensityShader::~LightIntensityShader()
{
}
Vec4f LightIntensityShader::vertex(int iface, int nthVert)
{
	ObjModel& model = *(gl_Device.model);
	Vec4f v = vec4f_SetPoint(model.vert(iface, nthVert));
	v = mat4x4_Mul_Vec4f(gl_Device.modelMat, v);// to world space
	v = mat4x4_Mul_Vec4f(gl_Device.uniform_projectionMat, v); // to camera space, and then to cvv
	v = mat4x4_Mul_Vec4f(gl_Device.viewPortMat, v); //to window space

	gl_Device.varying_uvs[nthVert] = model.uvs(iface, nthVert);
	projVec(mat4x4_Mul_Vec4f(gl_Device.inverseModelMat,
		vec4f_SetVec(model.vns(iface, nthVert))), &gl_Device.varying_vns[nthVert]);

	return Vec4f(v.x / v.w, v.y / v.w, v.z / v.w, v.w);
}
bool LightIntensityShader::fragment(const Vec3f& bar, Color& color)
{
	Vec3f nor = vec3f_Add(vec3f_Mul(gl_Device.varying_vns[0], bar.x),
		vec3f_Mul(gl_Device.varying_vns[1], bar.y),
		vec3f_Mul(gl_Device.varying_vns[2], bar.z));

	nor.normalize();
	float intensity = vec3f_Dot(-nor, gl_Device.light_Dir);

	if (intensity > .85) intensity = 1;
	else if (intensity > .60) intensity = .80;
	else if (intensity > .45) intensity = .60;
	else if (intensity > .30) intensity = .45;
	else if (intensity > .15) intensity = .30;
	else intensity = 0;

	color = Color(255,155,0)*intensity;

	return false;
}

NormalMapShader::~NormalMapShader()
{
}
Vec4f NormalMapShader::vertex(int iface, int nthVert)
{
	ObjModel& model = *(gl_Device.model);
	Vec4f v = vec4f_SetPoint(model.vert(iface, nthVert));
	v = mat4x4_Mul_Vec4f(gl_Device.modelMat, v);// to world space
	v = mat4x4_Mul_Vec4f(gl_Device.uniform_projectionMat, v); // to camera space, and then to cvv
	v = mat4x4_Mul_Vec4f(gl_Device.viewPortMat, v); //to window space

	gl_Device.varying_uvs[nthVert] = model.uvs(iface, nthVert);

	return Vec4f(v.x / v.w, v.y / v.w, v.z / v.w, v.w);
}
bool NormalMapShader::fragment(const Vec3f& bar, Color& color)
{

	Vec3f uv = vec3f_baryInterpolation(gl_Device.varying_uvs[0],
		gl_Device.varying_uvs[1], gl_Device.varying_uvs[2],bar);

	Vec3f nor = gl_Device.normalTexture->getVec3f(uv.x,(1.-uv.y));
	projVec(mat4x4_Mul_Vec4f(gl_Device.inverseModelMat,vec4f_SetVec(nor)),&nor);

	//nor.normalize();
	float intensity = flt_Clamp(vec3f_Dot(-nor, gl_Device.light_Dir),0.f,1.f);
	color = gl_Device.texture->getColor(uv.x, (1. - uv.y)) * intensity;


	return false;
}

PhongShader::~PhongShader()
{
}
Vec4f PhongShader::vertex(int iface, int nthVert)
{
	ObjModel& model = *(gl_Device.model);
	Vec4f v = vec4f_SetPoint(model.vert(iface, nthVert));
	gl_Device.varying_w[nthVert] = v = mat4x4_Mul_Vec4f(gl_Device.modelMat, v);// to world space
	v = mat4x4_Mul_Vec4f(gl_Device.uniform_projectionMat, v); // to camera space, and then to cvv
	v = mat4x4_Mul_Vec4f(gl_Device.viewPortMat, v); //to window space

	gl_Device.varying_uvs[nthVert] = model.uvs(iface, nthVert);

	return Vec4f(v.x / v.w, v.y / v.w, v.z / v.w, v.w);
}
bool PhongShader::fragment(const Vec3f& bar, Color& color)
{
	Vec3f uv = vec3f_baryInterpolation(gl_Device.varying_uvs[0],
		gl_Device.varying_uvs[1], gl_Device.varying_uvs[2], bar);

	Vec3f nor = gl_Device.normalTexture->getVec3f(uv.x, (1. - uv.y));
	projVec(mat4x4_Mul_Vec4f(gl_Device.inverseModelMat, vec4f_SetVec(nor)), &nor);
	nor.normalize();

	Vec3f l = -gl_Device.light_Dir; l.normalize();
	Vec3f reflect = vec3f_Reflect(nor,l);
	Vec3f eyeDir =  gl_Device.varying_eyePos;
	eyeDir.sub(vec3f_baryInterpolation(gl_Device.varying_w[0],
		gl_Device.varying_w[1], gl_Device.varying_w[2], bar));
	eyeDir.normalize();

	float diffuse = std::max(vec3f_Dot(l, nor),0.0f);
	float spec = std::pow(std::max(vec3f_Dot(eyeDir, reflect),0.0f),
		gl_Device.specularTexture->getSpecular(uv.x, 1.f - uv.y));
	color = gl_Device.texture->getColor(uv.x, (1. - uv.y)) ;

	float r = std::min(255.0f, 10.0f + color.r() * (k_d * diffuse+ k_s * spec));
	float g = std::min(255.0f, 10.0f + color.g() * (k_d * diffuse+ k_s * spec));
	float b = std::min(255.0f, 10.0f + color.b() * (k_d * diffuse+ k_s * spec));
	color.set(r,g,b);

	return false;
}


 /* tangent space normal mapping*/

TangentNormalShader::~TangentNormalShader()
{
}

Vec4f TangentNormalShader::vertex(int iface, int nthVert)
{
	ObjModel& model = *(gl_Device.model);
	Vec4f v = vec4f_SetPoint(model.vert(iface, nthVert));
	gl_Device.varying_w[nthVert] = v = mat4x4_Mul_Vec4f(gl_Device.modelMat, v);// to world space
	v = mat4x4_Mul_Vec4f(gl_Device.uniform_projectionMat, v); // to camera space, and then to cvv
	v = mat4x4_Mul_Vec4f(gl_Device.viewPortMat, v); //to window space

	gl_Device.varying_uvs[nthVert] = model.uvs(iface, nthVert);//save vertex uv

	Vec4f vn = vec4f_SetVec(model.vns(iface, nthVert));// save vertex normal
	vn = mat4x4_Mul_Vec4f(gl_Device.inverseModelMat,vec4f_SetVec(vn)); //transform normal
	projVec(vn,&gl_Device.varying_vns[nthVert]);
	
	return Vec4f(v.x / v.w, v.y / v.w, v.z / v.w, v.w);
}

bool TangentNormalShader::fragment(const Vec3f& bar, Color& color)
{
	Vec3f uv = vec3f_baryInterpolation(gl_Device.varying_uvs[0],
		gl_Device.varying_uvs[1], gl_Device.varying_uvs[2], bar);
	Vec3f nor = getTangentNormal(uv,bar);
	nor.normalize();

	Vec3f l = -gl_Device.light_Dir; l.normalize();
	Vec3f reflect = vec3f_Reflect(nor, l);
	Vec3f eyeDir = gl_Device.varying_eyePos;
	eyeDir.sub(vec3f_baryInterpolation(gl_Device.varying_w[0],
		gl_Device.varying_w[1], gl_Device.varying_w[2], bar));
	eyeDir.normalize();

	float diffuse = std::max(vec3f_Dot(l, nor), 0.0f);
	float spec = std::pow(std::max(vec3f_Dot(eyeDir, reflect),0.0f),
		gl_Device.specularTexture->getSpecular(uv.x, 1.f - uv.y));
	color = gl_Device.texture->getColor(uv.x, (1. - uv.y));


	float r = std::min(255.0f, 10.0f + color.r() * (1.2f * diffuse + 0.6f * spec));
	float g = std::min(255.0f, 10.0f + color.g() * (1.2f * diffuse + 0.6f * spec));
	float b = std::min(255.0f, 10.0f + color.b() * (1.2f * diffuse + 0.6f * spec));
	color.set(r, g, b);

	return false;
}

Vec3f TangentNormalShader::getTangentNormal(const Vec3f& uv,const Vec3f& bar)
{

	const Vec3f& p0 = gl_Device.varying_w[0];
	const Vec3f& p1 = gl_Device.varying_w[1];
	const Vec3f& p2 = gl_Device.varying_w[2];

	Vec3f nor = vec3f_baryInterpolation(gl_Device.varying_vns[0],
		gl_Device.varying_vns[1], gl_Device.varying_vns[2],bar);
	nor.normalize();

	Vec3f tangentCoord = gl_Device.tangentNormalTexture->getVec3f(uv.x,(1.f-uv.y));
	Mat3x3 m = Mat3x3(p1.x - p0.x, p1.y - p0.y, p1.z - p0.z,
					  p2.x - p0.x, p2.y - p0.y, p2.z - p0.z,
					  nor.x, nor.y, nor.z);

	Vec3f u;
	u.x = gl_Device.varying_uvs[1].x - gl_Device.varying_uvs[0].x;
	u.y = gl_Device.varying_uvs[2].x - gl_Device.varying_uvs[0].x;
	u.z = 0;

	Vec3f v;
	v.x = gl_Device.varying_uvs[1].y - gl_Device.varying_uvs[0].y;
	v.y = gl_Device.varying_uvs[2].y - gl_Device.varying_uvs[0].y;
	v.z = 0;

	m = mat3x3_Inverse(m);
	//tmp = mat3x3_Mul(m,tmp);
	u = mat3x3_Mul_Vec3f(m, u);
	v = mat3x3_Mul_Vec3f(m, v);
	u.normalize(); v.normalize();

	u.mult(tangentCoord.x);
	v.mult(tangentCoord.y);
	nor.mult(tangentCoord.z);

	return Vec3f(u.x+v.x+nor.x,u.y+v.y+nor.y,u.z+v.z+nor.z);
}

DepthShader::~DepthShader()
{
}

Vec4f DepthShader::vertex(int iface, int nthVert)
{
	ObjModel& model = *(gl_Device.model);
	Vec4f v = vec4f_SetPoint(model.vert(iface, nthVert));
	v = mat4x4_Mul_Vec4f(gl_Device.modelMat, v);// to world space
	v = mat4x4_Mul_Vec4f(gl_Device.uniform_projectionMat, v); // to camera space, and then to cvv
	v = mat4x4_Mul_Vec4f(gl_Device.viewPortMat, v); //to window space

	v.x /= v.w;
	v.y /= v.w;
	v.z /= v.w;
	v.w = 1 / v.w;

	gl_Device.varying_v[nthVert] = v;

	return v;
}
bool DepthShader::fragment(const Vec3f& bar, Color& color)
{
	float z = bar.x * gl_Device.varying_v[0].w + 
			  bar.y * gl_Device.varying_v[1].w + 
			  bar.z * gl_Device.varying_v[2].w;
	z = 1 / z;

	float n = gl_Device.varying_eyePos.z - 1.2f;
	float f = gl_Device.varying_eyePos.z + 1.2f;
	float c =  (z - n) /(f-n);

	color = Color(255,255,255)*c;

	return false;
}

ShadowShader::~ShadowShader()
{
}

Vec4f ShadowShader::vertex(int iface, int nthVert)
{

	ObjModel& model = *(gl_Device.model);
	Vec4f v = vec4f_SetPoint(model.vert(iface, nthVert));
	v = mat4x4_Mul_Vec4f(gl_Device.modelMat, v);// to world space
	gl_Device.varying_w[nthVert] = v;
	v = mat4x4_Mul_Vec4f(gl_Device.uniform_projectionMat, v); // to camera space, and then to cvv
	v = mat4x4_Mul_Vec4f(gl_Device.viewPortMat, v); //to window space
	perspectiveDivision(&v);

	gl_Device.varying_v[nthVert] = v;
	gl_Device.varying_uvs[nthVert] = model.uvs(iface, nthVert);//save vertex uv

	return v;
}

bool ShadowShader::fragment(const Vec3f& bar, Color& color)
{
	Vec3f uv = vec3f_baryInterpolation(gl_Device.varying_uvs[0],
		gl_Device.varying_uvs[1], gl_Device.varying_uvs[2], bar);

	Vec3f nor = gl_Device.normalTexture->getVec3f(uv.x, (1.0f - uv.y));
	projVec(mat4x4_Mul_Vec4f(gl_Device.inverseModelMat, vec4f_SetVec(nor)), &nor);
	nor.normalize();

	Vec3f l = -gl_Device.light_Dir; l.normalize();
	Vec3f reflect = vec3f_Reflect(nor, l);
	Vec3f eyeDir = gl_Device.varying_eyePos;
	eyeDir.sub(vec3f_baryInterpolation(gl_Device.varying_w[0],
		gl_Device.varying_w[1], gl_Device.varying_w[2], bar));
	eyeDir.normalize();

	float diffuse = std::max(vec3f_Dot(l, nor), 0.0f);
	float spec = std::pow(std::max(vec3f_Dot(eyeDir, reflect), 0.0f),
		gl_Device.specularTexture->getSpecular(uv.x, 1.f - uv.y));
	float shadow = getShadowFactor(nor,l,bar);
	color = gl_Device.texture->getColor(uv.x, (1. - uv.y));


	float r = std::min(255.0f, 10.0f + color.r() * shadow * (k_d * diffuse + k_s * spec));
	float g = std::min(255.0f, 10.0f + color.g() * shadow * (k_d * diffuse + k_s * spec));
	float b = std::min(255.0f, 10.0f + color.b() * shadow * (k_d * diffuse + k_s * spec));
	color.set(r, g, b);

	return false;
}

float ShadowShader::getShadowFactor(const Vec3f& nor, const Vec3f& lightDir, const Vec3f& bar)
{
	Vec3f v = vec3f_baryInterpolation(gl_Device.varying_w[0],
		gl_Device.varying_w[1], gl_Device.varying_w[2], bar);
	Vec4f hv = mat4x4_Mul_Vec4f(gl_Device.lightSpaceMat,vec4f_SetPoint(v));
	perspectiveDivision(&hv);

	Vec2Int index;
	projVec(hv,&index);
	index.x = flt_Clamp(index.x, 0, gl_Device.screen_width - 1);
	index.y = flt_Clamp(index.y, 0, gl_Device.screen_height - 1);

	// prevent Moiré-like pattern ,or the shadow acne will  come out
	// what is shadow acne ? https://computergraphics.stackexchange.com/questions/2192/cause-of-shadow-acne
	//beside by cull front face ,we also can reduce shadow acne 
	//制作shadow map时，为什么剔除正面可以解决物体悬浮问题（Peter panning问题）？ - 挖坑的费马的回答 - 知乎
	//https://www.zhihu.com/question/321779117/answer/794188615
	//float bias = std::max(0.05f*(1.0f - vec3f_Dot(nor,lightDir)), 0.0001f);
	bool inShadow = hv.z - 0.0001f > gl_Device.shadowBuf[index.y][index.x];

	return inShadow?0.0f:1.0f;
}

Vec4f FlatShader::vertex(int iface, int nthVert)
{
	ObjModel& model = *(gl_Device.model);
	Vec4f v = vec4f_SetPoint(model.vert(iface, nthVert));
	gl_Device.varying_w[nthVert] = v;
	v = mat4x4_Mul_Vec4f(gl_Device.modelMat, v);// to world space
	v = mat4x4_Mul_Vec4f(gl_Device.uniform_projectionMat, v); // to camera space, and then to cvv
	v = mat4x4_Mul_Vec4f(gl_Device.viewPortMat, v); //to window space

	perspectiveDivision(&v);
	gl_Device.varying_v[nthVert] = v;
	gl_Device.varying_uvs[nthVert] = model.uvs(iface,nthVert);

	return v;
}

bool FlatShader::fragment(const Vec3f& bar, Color& color)
{

	Vec3f v10 = vec3f_Sub(gl_Device.varying_w[1], gl_Device.varying_w[0]);
	Vec3f v20 = vec3f_Sub(gl_Device.varying_w[2], gl_Device.varying_w[0]);

	Vec3f lightDir = -gl_Device.light_Dir;
	Vec3f nor = vec3f_Normalize(vec3f_Cross(v10, v20));
	float shadow = getShadowFactor(nor,lightDir,bar);
	float diffuse = std::max(vec3f_Dot(lightDir, nor),0.0f);
	
	Vec3f uv = vec3f_baryInterpolation(gl_Device.varying_uvs[0], 
		gl_Device.varying_uvs[1], gl_Device.varying_uvs[2],bar);

	float r = std::min(255.0f, 50.0f + 255 * uv.x * shadow * (1.2f * diffuse ));
	float g = std::min(255.0f, 50.0f + 255 * uv.y * shadow * (1.2f * diffuse ));
	float b = std::min(255.0f, 50.0f + 255 * uv.z * shadow * (1.2f * diffuse));
	color.set(r, g, b);

	return false;
}

FlatShader::~FlatShader()
{
}

float FlatShader::getShadowFactor(const Vec3f& nor, const Vec3f& lightDir, const Vec3f& bar)
{
	Vec3f v = vec3f_baryInterpolation(gl_Device.varying_v[0],
		gl_Device.varying_v[1], gl_Device.varying_v[2], bar);
	Vec4f hv = mat4x4_Mul_Vec4f(gl_Device.mvpInvMat, vec4f_SetPoint(v));
	perspectiveDivision(&hv);
	hv = mat4x4_Mul_Vec4f(gl_Device.lightSpaceMat, hv);
	perspectiveDivision(&hv);
	Vec2Int index;
	projVec(hv, &index);
	index.x = flt_Clamp(index.x, 0, gl_Device.screen_width - 1);
	index.y = flt_Clamp(index.y, 0, gl_Device.screen_height - 1);

	

	// prevent shadow acne( Moiré-like pattern) by use shadow bias
	//why it works? --> https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping
	float bias = std::max(0.05f * (1.0f - vec3f_Dot(nor, lightDir)), 0.005f);
	bool inShadow = hv.z-bias  > gl_Device.shadowBuf[index.y][index.x];

	return inShadow ? 0.0f : 1.0f;
}
