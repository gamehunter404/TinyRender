#ifndef __CAMERA_H__
#define __CAMERA_H__
#include"Maths.h"

class Camera
{
public:
	Camera(Vec3f pos, Vec3f target, Vec3f up);
	Camera();
	~Camera();

	void setPos(Vec3f pos);
	void setTarget(Vec3f target);
	void setUp(Vec3f up);

	Vec3f get_CameraPos() const;
	Vec3f get_CameraTarget() const;
	Vec3f get_UpDir() const;
	Vec3f get_LookDir() const;

private:
	Vec3f _pos;
	Vec3f _target;
	Vec3f _up;

};
#endif // !__CAMERA_H__
