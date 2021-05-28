#include"Render.h"
#include"Device.h"
#include"Model.h"
#include"Maths.h"
#include"Camera.h"
#include"TextureManager.h"
#include<iostream>

void outpuVertex(int iface,int nthVertex,const Vec4f& v)
{
	std::cerr << "iface : " << iface << ", nthVer: " 
		<< nthVertex << " ("<<" "<<v.x<<" , "<<v.y
		<<" , "<<v.z<<" , "<<v.w<<" )"<<std::endl;
}
void Render::SetPixel(int x, int y, const Color& color)
{
	if (x < 0 || x >= gl_Device.screen_width) return;
	if (y<0 || y>= gl_Device.screen_height) return;

	// i = y,j = x
	gl_Device.frameBuf[y][x] = color.Data();
}
void Render::DrawLine(int x0, int y0, int x1, int y1,const Color& color)
{
	bool isStep = false;

	if (std::abs(x0 - x1) < std::abs(y1 - y0))
	{
		isStep = true;
		std::swap(x0, y0);
		std::swap(x1, y1);
	}


	if (x0 > x1)
	{
		std::swap(x0,x1);
		std::swap(y1,y0);
	}

	int dx = x1 - x0;
	int dy = y1 - y0;
	int derror2 = 2*std::abs(dy);
	int error2 = 0;

	

	for (int x = x0,y = y0; x <=x1; x++)
	{
		if (isStep) {
			SetPixel(y,x,color);
		}
		else {
			SetPixel(x,y,color);
		}

		error2 += derror2;
		if (error2 > dx) {
			y += (y1 > y0 ? 1 : -1);
			error2 -= dx * 2;
		}
	}
}
void Render::DrawTriangle(Vec3f*w, Vec2Int*v, Color& color)
{

	switch (gl_Device.triangleFillSetting)
	{
	case TriangleFillAlgorithm::TRIFILL_SWEEPING:
		drawTriBySweeping(w,v,color);
		break;

	case TriangleFillAlgorithm::TRIFILL_EDGEEQUATION:
		drawTriByEdgeEquation(w,v,color);
		break;
	}

}
void Render::DrawTriangle(Vec3f* w, Vec2Int* v, Vec3f* uvs)
{

	switch (gl_Device.triangleFillSetting)
	{
	case TriangleFillAlgorithm::TRIFILL_SWEEPING:
		break;

	case TriangleFillAlgorithm::TRIFILL_EDGEEQUATION:
		drawTriByEdgeEquation(w, v, uvs);
		break;
	}

}
void Render::renderWithShader(const Camera& camera, std::vector<Model>& models)
{
	if (gl_Device.shader == nullptr) return;

	IShader& shader = *(gl_Device.shader);
	Vec4f w[3];
	

	gl_Device.varying_eyePos = camera.getPos();
	gl_Device.viewMat = getViewMat(camera.getPos(), camera.getTarget(), camera.getUp()); 
	gl_Device.uniform_projectionMat = mat4x4_Mul(getProjectionMat(gl_Device), gl_Device.viewMat); 
	gl_Device.mvpInvMat = mat4x4_Mul(gl_Device.viewPortMat, gl_Device.uniform_projectionMat);

	for (int n = 0; n < models.size(); n++)
	{
		Model& model = models[n];
		gl_Device.modelMat = getModelMat(model.getScale(), model.getRotation(), model.getTranslate());
		gl_Device.mvpInvMat = mat4x4_Mul(gl_Device.mvpInvMat,gl_Device.modelMat); //gl_Device.mvpInvMat * gl_Device.modelMat;
		gl_Device.mvpInvMat = mat4x4_Inverse(gl_Device.mvpInvMat);
		gl_Device.inverseModelMat = getInverseModelMat(model.getScale(), model.getRotation(), model.getTranslate());
		gl_Device.texture = TextureManager::getTexture(model.getTextureName().c_str());
		gl_Device.normalTexture = TextureManager::getTexture(model.getNormalTextureName().c_str());
		gl_Device.specularTexture = TextureManager::getTexture(model.getSpecTextureName().c_str());
		gl_Device.tangentNormalTexture = TextureManager::getTexture(model.getTangentTextureName().c_str());
		gl_Device.model = &model;


		// generate shadow map
		if (gl_Device.generateShadowMap)
			renderShadow(model,gl_Device);

		for (int i = 0; i < model.nfaces(); i++)
		{
			for (int k = 0; k < 3; k++)
			{
				w[k] = shader.vertex(i,k);
			}

			switch (gl_Device.faceCulling)
			{
			case FaceCulling::FACECULLING_NONE:
				rasterizedTriangle(w,shader);
				break;
			case FaceCulling::FACECULLING_BACK:
				if (!isBackFace(w[0], w[1], w[2]))
					rasterizedTriangle(w, shader);
				break;
			case FaceCulling::FACECULLING_FRONT:
				if (isBackFace(w[0], w[1], w[2]))
					rasterizedTriangle(w, shader);
				break;
			}
		}

	}

	gl_Device.normalTexture = nullptr;
	gl_Device.texture = nullptr;
}
void Render::renderModel(const Camera& camera,std::vector<Model>& models)
{
	gl_Device.viewMat = getViewMat(camera.getPos(),camera.getTarget(),camera.getUp()); 
	gl_Device.uniform_projectionMat = mat4x4_Mul(getProjectionMat(gl_Device), gl_Device.viewMat); 

	Vec4f w[3];
	Vec3f uvs[3];
	Vec3f vns[3];
	Vec4f v[3];

	for (int n = 0; n < models.size(); n++)
	{
		Model& model = models[n];
		gl_Device.modelMat = getModelMat(model.getScale(),model.getRotation(),model.getTranslate());
		gl_Device.inverseModelMat = getInverseModelMat(model.getScale(), model.getRotation(), model.getTranslate());
		gl_Device.texture = TextureManager::getTexture(model.getTextureName().c_str());

		for (int i = 0; i < model.nfaces(); i++)
		{
			auto face = model.face(i);
			w[0] = vec4f_SetPoint(model.vert(face[0].verIndex));
			w[1] = vec4f_SetPoint(model.vert(face[1].verIndex));
			w[2] = vec4f_SetPoint(model.vert(face[2].verIndex));

			uvs[0] = model.uvs(face[0].vtIndex);
			uvs[1] = model.uvs(face[1].vtIndex);
			uvs[2] = model.uvs(face[2].vtIndex);

			vns[0] = model.vns(face[0].vnIndex);
			vns[1] = model.vns(face[1].vnIndex);
			vns[2] = model.vns(face[2].vnIndex);

			w[0] = mat4x4_Mul_Vec4f(gl_Device.modelMat, w[0]);
			w[1] = mat4x4_Mul_Vec4f(gl_Device.modelMat, w[1]);
			w[2] = mat4x4_Mul_Vec4f(gl_Device.modelMat, w[2]);

			projVec(mat4x4_Mul_Vec4f(gl_Device.inverseModelMat, 
				vec4f_SetVec(vns[0])),&vns[0]);
			projVec(mat4x4_Mul_Vec4f(gl_Device.inverseModelMat, 
				vec4f_SetVec(vns[1])), &vns[1]);
			projVec(mat4x4_Mul_Vec4f(gl_Device.inverseModelMat,
				vec4f_SetVec(vns[2])),&vns[2]);

			v[0] = mat4x4_Mul_Vec4f(gl_Device.uniform_projectionMat, w[0]);
			v[1] = mat4x4_Mul_Vec4f(gl_Device.uniform_projectionMat, w[1]);
			v[2] = mat4x4_Mul_Vec4f(gl_Device.uniform_projectionMat, w[2]);

			v[0] = mat4x4_Mul_Vec4f(gl_Device.viewPortMat, v[0]);
			v[1] = mat4x4_Mul_Vec4f(gl_Device.viewPortMat, v[1]);
			v[2] = mat4x4_Mul_Vec4f(gl_Device.viewPortMat, v[2]);
			v[0].x /= v[0].w,v[0].y /= v[0].w,v[0].z /= v[0].w;
			v[1].x /= v[1].w,v[1].y /= v[1].w,v[1].z /= v[1].w;
			v[2].x /= v[2].w,v[2].y /= v[2].w,v[2].z /= v[2].w;

			/*outpuVertex(i, 0, v[0]);
			outpuVertex(i, 1, v[1]);
			outpuVertex(i, 2, v[2]);*/

			if (!isBackFace(v[0], v[1], v[2]))
			{
				drawTriByEdgeEquation(w, v, uvs, vns, model);
			}
		}

	}

	
	gl_Device.texture = nullptr;
}
void Render::renderShadow(Model& model,Device& device)
{
	if (device.shadowBuf == nullptr) return;
	
	//shadow mapping
	//https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping


	Vec4f w[3];
	Vec2Int v[3];

	float t = 1.5f;
	float b = -t;
	float r = device.screenRatio * t;
	float l = -r;
	float n = 1.0f;
	float f = 7.5f;
	FaceCulling cullingModel = FaceCulling::FACECULLING_FRONT;

	device.lightSpaceMat = getViewPortMat(device.screen_width, device.screen_height);
	
	//Because we're modelling a directional light source, 
	//all its light rays are parallel. For this reason, 
	//we're going to use an orthographic projection matrix for the light source where there is no perspective deform
	device.lightSpaceMat.mult(getOrthogonalMat(Vec4f(l,r,b,t),n,f));

	//Under the condition of parallel light, 
	//the translation of the object will not affect the generation of ShadowMap.
	// so we can assume that the obj is at (0,0,0)
	// beside the light`s pos may be in the obj，so we scale it by 100
	device.lightSpaceMat.mult(getViewMat(-gl_Device.light_Dir * 3, Vec3f(0, 0, 0), Vec3f(0, 1, 0)));
	device.lightSpaceMat.mult(getModelMat(model.getScale(), model.getRotation(),Vec3f(0,0,0)));

	for (int k = 0; k < model.nfaces(); k++)
	{
		for (int i = 0; i < 3; i++)
		{
			w[i] = vec4f_SetPoint(model.vert(k, i));
			w[i] = mat4x4_Mul_Vec4f(device.lightSpaceMat,w[i]);
			w[i] = perspectiveDivision(w[i]);
			projVec(w[i],&v[i]);
		}

		bool backFace = isBackFace(w[0], w[1], w[2]);

		if (cullingModel == FaceCulling::FACECULLING_BACK && backFace)
			continue;

		if (cullingModel == FaceCulling::FACECULLING_FRONT && !backFace)
			continue;



		BoundingBox2D bb = BoundingBox2D(v, 3, 0,
			device.screen_width - 1, 0, device.screen_height - 1);

		for (int y = bb.minY; y <= bb.maxY; ++y)
		{
			for (int x = bb.minX; x <= bb.maxX; ++x)
			{
				Vec3f bar = getBarycentric2D(v[0],v[1],v[2],Vec2Int(x,y));

				if (bar.x < 0 || bar.y < 0 || bar.z < 0)
					continue;

				float z = bar.x * w[0].z + bar.y * w[1].z + bar.z * w[2].z;

				if (z < device.shadowBuf[y][x])
				{
					device.shadowBuf[y][x] = z;
				}
			}
		}
	}
}
void Render::drawTriByEdgeEquation(Vec3f* w, Vec2Int* v, const Color& color)
{

	Vec2Int& v0 = v[0];
	Vec2Int& v1 = v[1];
	Vec2Int& v2 = v[2];

	int minX = std::min(std::min(v0.x,v1.x),v2.x), maxX = std::max(std::max(v0.x, v1.x), v2.x);
	int minY = std::min(std::min(v0.y, v1.y), v2.y), maxY = std::max(std::max(v0.y,v1.y),v2.y);
	
	minX = std::max(0,minX);
	maxX = std::min(gl_Device.screen_width-1,maxX);
	minY = std::max(0,minY);
	maxY = std::min(gl_Device.screen_height-1,maxY);

	if (v0.y == v1.y && v2.y == v1.y) {

		DrawLine(minX,v0.y,maxX,v0.y,color);
		return;
	}
	else if (v0.x == v1.x && v1.x == v2.x)
	{
		DrawLine(v0.x,minY,v0.x,maxY,color);
		return;
	}

	float z = 0;

	for (int y = minY; y <= maxY; y++)
	{
		for (int x = minX; x <= maxX; x++)
		{
			Vec3f u = getBarycentric2D(v0, v1, v2, {x,y});


			if (u.x < 0 || u.y < 0 || u.z < 0) 
				continue;

			z = w[0].z * u.x+w[1].z*u.y+w[2].z*u.z;

			if (z > gl_Device.zBuf[y][x])
			{
				gl_Device.zBuf[y][x] = z;
				SetPixel(x, y, color);
			}

		}
	}
}
void Render::drawTriByEdgeEquation(Vec3f* w, Vec2Int* v, Vec3f* uvs)
{
	Vec2Int& v0 = v[0];
	Vec2Int& v1 = v[1];
	Vec2Int& v2 = v[2];
	Color color = Color(255,255,255,255);
	
	int minX = std::min(std::min(v0.x, v1.x), v2.x), maxX = std::max(std::max(v0.x, v1.x), v2.x);
	int minY = std::min(std::min(v0.y, v1.y), v2.y), maxY = std::max(std::max(v0.y, v1.y), v2.y);

	minX = std::max(0, minX);
	maxX = std::min(gl_Device.screen_width - 1, maxX);
	minY = std::max(0, minY);
	maxY = std::min(gl_Device.screen_height - 1, maxY);

	float z = 0;

	for (int y = minY; y <= maxY; y++)
	{
		for (int x = minX; x <= maxX; x++)
		{
			Vec3f u = getBarycentric2D(v0, v1, v2, { x,y });

			if (u.x < 0 || u.y < 0 || u.z < 0)
				continue;

			z = w[0].z * u.x + w[1].z * u.y + w[2].z * u.z;
			

			if (z > gl_Device.zBuf[y][x])
			{
				Vec3f uv = interpolateUVCoord(uvs, u);

				gl_Device.zBuf[y][x] = z;
				//SetPixel(x, y, Color(tgaColor.bgra[2], tgaColor.bgra[1], tgaColor.bgra[0], 255), device);
			}

		}
	}
}
void Render::drawTriByEdgeEquation(Vec4f* w, Vec4f* v, Vec3f* uvs,Vec3f*vns,const Model&model)
{

	Vec2Int v0 = Vec2Int(v[0].x, v[0].y);
	Vec2Int v1 = Vec2Int(v[1].x, v[1].y);
	Vec2Int v2 = Vec2Int(v[2].x, v[2].y);

	int minX = std::min(std::min(v0.x, v1.x), v2.x), maxX = std::max(std::max(v0.x, v1.x), v2.x);
	int minY = std::min(std::min(v0.y, v1.y), v2.y), maxY = std::max(std::max(v0.y, v1.y), v2.y);
	minX = std::max(0, minX);
	maxX = std::min(gl_Device.screen_width - 1, maxX);
	minY = std::max(0, minY);
	maxY = std::min(gl_Device.screen_height - 1, maxY);
	
	float z = 0;
	float inverseW0 = 1 / v[0].z;
	float inverseW1 = 1 / v[1].z;
	float inverseW2 = 1 / v[2].z;

	for (int y = minY; y <= maxY; y++)
	{
		for (int x = minX; x <= maxX; x++)
		{
			Vec3f u = getBarycentric2D(v0, v1, v2, { x,y });

			if (u.x < 0 || u.y < 0 || u.z < 0)
				continue;

			z = inverseW0 * u.x + inverseW1 * u.y + inverseW2 * u.z;
			z = 1. / z;

			if (z > gl_Device.zBuf[y][x])
			{
				Vec3f nor = -vec3f_baryInterpolation(vns[0], vns[1], vns[2], u);
				nor.normalize();
				float light_intensity = vec3f_Dot(nor,gl_Device.light_Dir);
				Vec3f uv = vec3f_baryInterpolation(uvs[0], uvs[1], uvs[2], u);
				gl_Device.zBuf[y][x] = z;

				Color col = gl_Device.texture->getColor(uv.x,(1. - uv.y))* light_intensity;
				SetPixel(x, y, col);
			}

		}
	}

}

void Render::rasterizedTriangle(Vec4f*w,IShader&shader)
{
	if (w == nullptr)
		return;

	Vec2Int v[3]; 
	projVec(w[0], &v[0]);
	projVec(w[1], &v[1]);
	projVec(w[2], &v[2]);

	Color color(255, 255, 255, 255);
	BoundingBox2D bbox(v, 3, 0, gl_Device.screen_width-1, 0, gl_Device.screen_height-1);
	int minX = static_cast<int>(bbox.minX);
	int maxX = static_cast<int>(bbox.maxX);
	int minY = static_cast<int>(bbox.minY);
	int maxY = static_cast<int>(bbox.maxY);

	static int count = 0;
	count++;

	for (int y = minY; y <= maxY; y++)
	{
		for (int x = minX; x <= maxX; x++)
		{
			Vec3f bar = getBarycentric2D(v[0],v[1],v[2],Vec2Int(x,y));

			if (bar.x < 0 || bar.y < 0 || bar.z < 0) 
				continue;

			gl_Device.varying_PixelDepth = w[0].z * bar.x + w[1].z * bar.y + w[2].z * bar.z;

			bool discard = shader.fragment(bar, color);
			
			if (gl_Device.varying_PixelDepth < gl_Device.zBuf[y][x])
			{
				gl_Device.zBuf[y][x] = gl_Device.varying_PixelDepth;
				SetPixel(x, y, color);
			}
		}
	}
}

void Render::drawTriBySweeping(Vec3f* w, Vec2Int* v, const Color& color)
{
	Vec2Int& v0 = v[0];
	Vec2Int& v1 = v[1];
	Vec2Int& v2 = v[2];

	if (v0.y > v1.y) std::swap(v0, v1);
	if (v0.y > v2.y) std::swap(v0, v2);
	if (v1.y > v2.y) std::swap(v1, v2);

	int x0 = v0.x, x1 = v0.x;

	if (v0.y == v1.y && v1.y == v2.y) {

		x0 = std::min(std::min(v0.x, v1.x), v2.x);
		x1 = std::max(std::max(v0.x, v1.x), v2.x);
		DrawLine(x0, v0.y, x1, v0.y, color);
		return;
	}

	for (int y = v0.y; y <= v2.y; y++)
	{

		float t1 = (float)(y - v0.y) / (v2.y - v0.y);
		float t2 = 0;
		x0 = v0.x + t1 * (v2.x - v0.x);

		if (y >= v1.y)
		{
			t2 = (v2.y - v1.y) != 0 ? (float)(y - v1.y) / (v2.y - v1.y) : 0;
			x1 = v1.x + t2 * (v2.x - v1.x);

		}
		else {
			t2 = (float)(y - v0.y) / (v1.y - v0.y);
			x1 = v0.x + t2 * (v1.x - v0.x);
		}

		if (x0 > x1) std::swap(x0, x1);

		for (int x = x0; x <= x1; x++)
		{
			SetPixel(x, y, color);
		}

	}
}

Vec3f Render::interpolateUVCoord(Vec3f* uv,const Vec3f& vuw)
{
	return Vec3f(uv[0].x * vuw.x + uv[1].x * vuw.y + uv[2].x * vuw.z,
				 uv[0].y * vuw.x + uv[1].y * vuw.y + uv[2].y * vuw.z,
				 uv[0].z * vuw.x + uv[1].z * vuw.y + uv[2].z * vuw.z);
}

Vec3f Render::getBarycentric2D(const Vec2Int& v0, const Vec2Int& v1, const Vec2Int& v2, const Vec2Int& p)
{

	Vec3f u = vec3f_Cross(Vec3f((v1.x - v0.x), (v2.x - v0.x), (v0.x - p.x)),Vec3f( (v1.y - v0.y),(v2.y - v0.y),(v0.y - p.y)));

	if (std::abs(u.z) < 1.f)
	{
		// because u.z is a integer ，u.z<1.f means u.z is zero
		return {-1,1,1};
	}

	return { 1.f - (u.x + u.y) / (u.z) ,u.x/u.z,u.y/u.z};
}

 
/*
this formula is same as :
			vec2f_Cross(Vec2f((v1.x - v0.x),(v1.y- v0.y)),Vec2f((v2.x - v0.x),(v2.y - v0.y)))
*/
bool Render::isBackFace(const Vec4f& v0, const Vec4f& v1, const Vec4f& v2)
{

	return ((v0.x * v1.y - v1.x * v0.y) +
		(v1.x * v2.y - v2.x * v1.y) +
		(v2.x * v0.y - v0.x * v2.y))>=0;
}
