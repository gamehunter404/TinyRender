#include"Render.h"
#include"Device.h"
#include"Model.h"
#include"Maths.h"
#include"Camera.h"
#include"TextureManager.h"
#include<iostream>


void Render::SetPixel(int x, int y, const Color& color)
{
	if (x < 0 || x >= gl_Device.sereen_width) return;
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

void Render::renderModel(const Camera& camera,std::vector<Model>& models)
{
	gl_Device.viewMat = getViewMat(camera.getPos(),camera.getTarget(),camera.getUp()); //视角矩阵
	gl_Device.vpMat = mat4x4_Mul(getProjectionMat(gl_Device), gl_Device.viewMat); //投影矩阵

	Vec4f w[3];//世界坐标
	Vec3f uvs[3];//顶点uv
	Vec3f vns[3];//顶点法线
	Vec4f v[3];
	Vec4f p[3];

	gl_Device.light_Dir = camera.lookDir();


	for (int n = 0; n < models.size(); n++)
	{
		Model& model = models[n];
		gl_Device.modelMat = getModelMat(model.getScale(),model.getRotation(),model.getTranslate());
		gl_Device.inverseModelMat = getInverseModelMat(model.getScale(), model.getRotation(), model.getTranslate());
		gl_Device.texture = TextureManager::getTexture(model.getTextureName().c_str());
		gl_Device.texWidth = gl_Device.texture->get_width();
		gl_Device.texHeight= gl_Device.texture->get_height();

		for (int i = 0; i < model.nfaces(); i++)
		{
			auto& face = model.face(i);
			w[0] = vec4f_SetPoint(model.vert(face[0].verIndex));
			w[1] = vec4f_SetPoint(model.vert(face[1].verIndex));
			w[2] = vec4f_SetPoint(model.vert(face[2].verIndex));

			uvs[0] = model.uvs(face[0].vtIndex);
			uvs[1] = model.uvs(face[1].vtIndex);
			uvs[2] = model.uvs(face[2].vtIndex);

			vns[0] = model.vns(face[0].vnIndex);
			vns[1] = model.vns(face[1].vnIndex);
			vns[2] = model.vns(face[2].vnIndex);

			w[0] = vec4f_Mul(gl_Device.modelMat, w[0]);
			w[1] = vec4f_Mul(gl_Device.modelMat, w[1]);
			w[2] = vec4f_Mul(gl_Device.modelMat, w[2]);

			vns[0] = vec4f_Mul(gl_Device.inverseModelMat, vec4f_SetVec(vns[0]));
			vns[1] = vec4f_Mul(gl_Device.inverseModelMat, vec4f_SetVec(vns[1]));
			vns[2] = vec4f_Mul(gl_Device.inverseModelMat, vec4f_SetVec(vns[2]));


			v[0] = vec4f_Mul(gl_Device.vpMat, w[0]);
			v[1] = vec4f_Mul(gl_Device.vpMat, w[1]);
			v[2] = vec4f_Mul(gl_Device.vpMat, w[2]);

			v[0].x /= v[0].w,v[0].y /= v[0].w,v[0].z /= v[0].w, v[0].w = 1;
			v[1].x /= v[1].w,v[1].y /= v[1].w,v[1].z /= v[1].w, v[1].w = 1;
			v[2].x /= v[2].w,v[2].y /= v[2].w,v[2].z /= v[2].w, v[2].w = 1;

			Vec3f n = vec3f_Cross(vec3f_Sub(v[2], v[0]), vec3f_Sub(v[1], v[0]));
		
			if (n.z < 0)
			{
				v[0] = vec4f_Mul(gl_Device.viewPortMat, v[0]);
				v[1] = vec4f_Mul(gl_Device.viewPortMat, v[1]);
				v[2] = vec4f_Mul(gl_Device.viewPortMat, v[2]);

				drawTriByEdgeEquation(w, v, uvs, vns, model);
			}
		}

	}

	gl_Device.texHeight = 0;
	gl_Device.texWidth = 0;
	gl_Device.texture = nullptr;
}

void Render::drawTriByEdgeEquation(Vec3f* w, Vec2Int* v, const Color& color)
{

	Vec2Int& v0 = v[0];
	Vec2Int& v1 = v[1];
	Vec2Int& v2 = v[2];

	int minX = std::min(std::min(v0.x,v1.x),v2.x), maxX = std::max(std::max(v0.x, v1.x), v2.x);
	int minY = std::min(std::min(v0.y, v1.y), v2.y), maxY = std::max(std::max(v0.y,v1.y),v2.y);
	
	minX = std::max(0,minX);
	maxX = std::min(gl_Device.sereen_width-1,maxX);
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
	maxX = std::min(gl_Device.sereen_width - 1, maxX);
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
	maxX = std::min(gl_Device.sereen_width - 1, maxX);
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

			//透视矫正
			z = inverseW0 * u.x + inverseW1 * u.y + inverseW2 * u.z;
			z = 1. / z;

			if (z > gl_Device.zBuf[y][x])
			{
				//TODO 贴图透视矫正
				Vec3f nor = -vec3f_Add(vns[0] * u.x, vns[1] * u.y, vns[2] * u.z);
				nor.normalize();
				float light_intensity = vec3f_Dot(nor,gl_Device.light_Dir);
				Vec3f uv = vec3f_Add(uvs[0] * u.x, uvs[1] * u.y, uvs[2] * u.z);
				gl_Device.zBuf[y][x] = z;

				Color col = gl_Device.texture->getColor(uv.x * gl_Device.texWidth,
					(1. - uv.y) * gl_Device.texHeight)* light_intensity;
				SetPixel(x, y, col);
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

		//三角形退化为一条水平直线
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
		//u.z <1 表示u.z的值为0。此时表示三角形已经退化
		return {-1,1,1};
	}

	return { 1.f - (u.x + u.y) / (u.z) ,u.x/u.z,u.y/u.z};
}
