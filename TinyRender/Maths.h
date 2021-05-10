#ifndef __MATHS_H__
#define __MATHS_H__


typedef struct { float x, y; } Vec2f;
typedef struct { float x, y, z; } Vec3f;
typedef struct { int x, y;} Vec2Int;
typedef struct { int x, y,z;} Vec3Int;
typedef struct { float m[3][3]; } Mat3x3;
typedef struct { float m[4][4]; } Mat4x4;



/*vector2 relate function*/
Vec2f Vec2_Add(Vec2f&a,Vec2f&b);
Vec2f Vec2_Sub(Vec2f&a,Vec2f&b);
Vec2f Vec2_Mul(Vec2f&a, float val);


/*vector2int relate funtion*/
Vec2Int Vec2i_Add(Vec2Int& a, Vec2Int& b);
Vec2Int Vec2i_Sub(Vec2Int& a, Vec2Int& b);


//m3x3 relate function
Mat3x3 RotationX(float radian);
Mat3x3 RotationY(float radian);
Mat3x3 RotationZ(float radian);


#endif // !__MATHS_H__




