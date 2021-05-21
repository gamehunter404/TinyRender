#include "Shader.h"
#include"Maths.h"
#include"Device.h"
#include<iostream>

Vec4f TextureShader::vertex(int iface, int nthVert)
{
	Model& model = *(gl_Device.model);
	Vec4f v = vec4f_SetPoint(model.vert(iface, nthVert));
	v = mat4x4_Mul_Vec4f(gl_Device.modelMat,v);// to world space
	v = mat4x4_Mul_Vec4f(gl_Device.uniform_vpMat,v); // to camera space, and then to cvv
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
	Model& model = *(gl_Device.model);
	Vec4f v = vec4f_SetPoint(model.vert(iface, nthVert));
	v = mat4x4_Mul_Vec4f(gl_Device.modelMat, v);// to world space
	v = mat4x4_Mul_Vec4f(gl_Device.uniform_vpMat, v); // to camera space, and then to cvv
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
	Model& model = *(gl_Device.model);
	Vec4f v = vec4f_SetPoint(model.vert(iface, nthVert));
	v = mat4x4_Mul_Vec4f(gl_Device.modelMat, v);// to world space
	v = mat4x4_Mul_Vec4f(gl_Device.uniform_vpMat, v); // to camera space, and then to cvv
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
	Model& model = *(gl_Device.model);
	Vec4f v = vec4f_SetPoint(model.vert(iface, nthVert));
	gl_Device.varying_w[nthVert] = v = mat4x4_Mul_Vec4f(gl_Device.modelMat, v);// to world space
	v = mat4x4_Mul_Vec4f(gl_Device.uniform_vpMat, v); // to camera space, and then to cvv
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
	float spec = std::pow(std::max(vec3f_Dot(eyeDir,reflect),0.0f), gl_Device.specularTexture->getSpecular(uv.x, 1.f - uv.y));
	color = gl_Device.texture->getColor(uv.x, (1. - uv.y)) ;


	float r = std::min(255.0f, 5.0f + color.r() * (k_d * diffuse+ k_s * spec));
	float g = std::min(255.0f, 5.0f + color.g() * (k_d * diffuse+ k_s * spec));
	float b = std::min(255.0f, 5.0f + color.b() * (k_d * diffuse+ k_s * spec));
	color.set(r,g,b);

	return false;
}
