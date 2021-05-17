#include"Camera.h"

Camera::Camera(Vec3f pos, Vec3f target, Vec3f up):_pos(pos), _target(target), _up(up)
{
}

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::setPos(Vec3f pos)
{
	_pos = pos;
}

void Camera::setTarget(Vec3f target)
{
	_target = target;
}

void Camera::setUp(Vec3f up)
{
	_up = up;
}

Vec3f Camera::getPos() const
{
	return _pos;
}

Vec3f Camera::getTarget() const
{
	return _target;
}

Vec3f Camera::getUp() const
{
	return _up;
}

Vec3f Camera::lookDir() const
{
	return vec3f_Normalize(vec3f_Sub(_target, _pos));
}
