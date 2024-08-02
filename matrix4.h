#ifndef  __MATRIX4_H 
#define  __MATRIX4_H 

struct Matrix4
{
	float m[4][4];
};

void SetZero(Matrix4& mat); 
void SetIdentity(Matrix4& mat);
void CreateOrthoProjection(Matrix4& mat, float width, float height);

#endif //   __MATRIX4_H 
