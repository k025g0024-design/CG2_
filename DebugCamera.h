#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include<dinput.h>

struct Matrix4x4
{
	float m[4][4];
};

struct Vector3
{
	float x;
	float y;
	float z;
};

//三次元アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

Matrix4x4 Inverse(const Matrix4x4& m);

Matrix4x4 MakeRotateZMatrix(float radian);

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

class DebugCamera
{
public:
	

	
	Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

	Vector3 translation_ = { 0,0,-50 };

	Matrix4x4 matRot_ = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1,
	};
	//ビュー行列
	Matrix4x4 viewMatrix =
	{

	};

	//射影行列
	Matrix4x4 projectionMatrix = {};

	Matrix4x4 MakeIdentityMatrix = 
	{
	};

public:

	void Initialize();

	void Update(BYTE *key);

};

