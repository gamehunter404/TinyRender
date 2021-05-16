#ifndef __CAMERA_H__
#define __CAMERA_H__
#include"Maths.h"

class Camera
{
public:
	Camera();
	~Camera();

	void setPos(Vec3f pos);
	void setTarget(Vec3f target);
	void setUp(Vec3f up);

	Vec3f getPos() const;
	Vec3f getTarget() const;
	Vec3f getUp() const;
	Vec3f lookDir() const;

private:
	Vec3f _pos;
	Vec3f _target;
	Vec3f _up;
};


#endif // !__CAMERA_H__
