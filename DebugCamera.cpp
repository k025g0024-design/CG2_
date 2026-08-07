#include "DebugCamera.h"
#include <cmath>

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate)
{

	float sx = std::sin(rotate.x);
	float cx = std::cos(rotate.x);
	float sy = std::sin(rotate.y);
	float cy = std::cos(rotate.y);
	float sz = std::sin(rotate.z);
	float cz = std::cos(rotate.z);

	Matrix4x4 result;

	// 0行目
	result.m[0][0] = scale.x * (cy * cz);
	result.m[1][0] = scale.y * (sx * sy * cz - cx * sz);
	result.m[2][0] = scale.z * (cx * sy * cz + sx * sz);
	result.m[3][0] = translate.x;

	// 1行目
	result.m[0][1] = scale.x * (cy * sz);
	result.m[1][1] = scale.y * (sx * sy * sz + cx * cz);
	result.m[2][1] = scale.z * (cx * sy * sz - sx * cz);
	result.m[3][1] = translate.y;

	// 2行目
	result.m[0][2] = scale.x * (-sy);
	result.m[1][2] = scale.y * (sx * cy);
	result.m[2][2] = scale.z * (cx * cy);
	result.m[3][2] = translate.z;

	// 3行目
	result.m[0][3] = 0.0f;
	result.m[1][3] = 0.0f;
	result.m[2][3] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}

Matrix4x4 Inverse(const Matrix4x4& m)
{
	Matrix4x4 result{};

	float det = 0.0f;

	det +=
		m.m[0][0] *
		(
			m.m[1][1] * (m.m[2][2] * m.m[3][3] - m.m[2][3] * m.m[3][2]) -
			m.m[1][2] * (m.m[2][1] * m.m[3][3] - m.m[2][3] * m.m[3][1]) +
			m.m[1][3] * (m.m[2][1] * m.m[3][2] - m.m[2][2] * m.m[3][1])
			);

	det -=
		m.m[0][1] *
		(
			m.m[1][0] * (m.m[2][2] * m.m[3][3] - m.m[2][3] * m.m[3][2]) -
			m.m[1][2] * (m.m[2][0] * m.m[3][3] - m.m[2][3] * m.m[3][0]) +
			m.m[1][3] * (m.m[2][0] * m.m[3][2] - m.m[2][2] * m.m[3][0])
			);

	det +=
		m.m[0][2] *
		(
			m.m[1][0] * (m.m[2][1] * m.m[3][3] - m.m[2][3] * m.m[3][1]) -
			m.m[1][1] * (m.m[2][0] * m.m[3][3] - m.m[2][3] * m.m[3][0]) +
			m.m[1][3] * (m.m[2][0] * m.m[3][1] - m.m[2][1] * m.m[3][0])
			);

	det -=
		m.m[0][3] *
		(
			m.m[1][0] * (m.m[2][1] * m.m[3][2] - m.m[2][2] * m.m[3][1]) -
			m.m[1][1] * (m.m[2][0] * m.m[3][2] - m.m[2][2] * m.m[3][0]) +
			m.m[1][2] * (m.m[2][0] * m.m[3][1] - m.m[2][1] * m.m[3][0])
			);

	// ゼロ除算を防止
	if (fabs(det) < 0.00001f)
	{
		return result; // 逆行列が存在しない場合はすべて0の行列を返す
	}

	float determinantRecp = 1.0f / det;

	// 各成分（余因子行列の転置 × 行列式の逆数）の計算
	result.m[0][0] = (m.m[1][1] * (m.m[2][2] * m.m[3][3] - m.m[2][3] * m.m[3][2]) - m.m[1][2] * (m.m[2][1] * m.m[3][3] - m.m[2][3] * m.m[3][1]) + m.m[1][3] * (m.m[2][1] * m.m[3][2] - m.m[2][2] * m.m[3][1])) * determinantRecp;
	result.m[0][1] = -(m.m[0][1] * (m.m[2][2] * m.m[3][3] - m.m[2][3] * m.m[3][2]) - m.m[0][2] * (m.m[2][1] * m.m[3][3] - m.m[2][3] * m.m[3][1]) + m.m[0][3] * (m.m[2][1] * m.m[3][2] - m.m[2][2] * m.m[3][1])) * determinantRecp;
	result.m[0][2] = (m.m[0][1] * (m.m[1][2] * m.m[3][3] - m.m[1][3] * m.m[3][2]) - m.m[0][2] * (m.m[1][1] * m.m[3][3] - m.m[1][3] * m.m[3][1]) + m.m[0][3] * (m.m[1][1] * m.m[3][2] - m.m[1][2] * m.m[3][1])) * determinantRecp;
	result.m[0][3] = -(m.m[0][1] * (m.m[1][2] * m.m[2][3] - m.m[1][3] * m.m[2][2]) - m.m[0][2] * (m.m[1][1] * m.m[2][3] - m.m[1][3] * m.m[2][1]) + m.m[0][3] * (m.m[1][1] * m.m[2][2] - m.m[1][2] * m.m[2][1])) * determinantRecp;

	result.m[1][0] = -(m.m[1][0] * (m.m[2][2] * m.m[3][3] - m.m[2][3] * m.m[3][2]) - m.m[1][2] * (m.m[2][0] * m.m[3][3] - m.m[2][3] * m.m[3][0]) + m.m[1][3] * (m.m[2][0] * m.m[3][2] - m.m[2][2] * m.m[3][0])) * determinantRecp;
	result.m[1][1] = (m.m[0][0] * (m.m[2][2] * m.m[3][3] - m.m[2][3] * m.m[3][2]) - m.m[0][2] * (m.m[2][0] * m.m[3][3] - m.m[2][3] * m.m[3][0]) + m.m[0][3] * (m.m[2][0] * m.m[3][2] - m.m[2][2] * m.m[3][0])) * determinantRecp;
	result.m[1][2] = -(m.m[0][0] * (m.m[1][2] * m.m[3][3] - m.m[1][3] * m.m[3][2]) - m.m[0][2] * (m.m[1][0] * m.m[3][3] - m.m[1][3] * m.m[3][0]) + m.m[0][3] * (m.m[1][0] * m.m[3][2] - m.m[1][2] * m.m[3][0])) * determinantRecp;
	result.m[1][3] = (m.m[0][0] * (m.m[1][2] * m.m[2][3] - m.m[1][3] * m.m[2][2]) - m.m[0][2] * (m.m[1][0] * m.m[2][3] - m.m[1][3] * m.m[2][0]) + m.m[0][3] * (m.m[1][0] * m.m[2][2] - m.m[1][2] * m.m[2][0])) * determinantRecp;

	result.m[2][0] = (m.m[1][0] * (m.m[2][1] * m.m[3][3] - m.m[2][3] * m.m[3][1]) - m.m[1][1] * (m.m[2][0] * m.m[3][3] - m.m[2][3] * m.m[3][0]) + m.m[1][3] * (m.m[2][0] * m.m[3][1] - m.m[2][1] * m.m[3][0])) * determinantRecp;
	result.m[2][1] = -(m.m[0][0] * (m.m[2][1] * m.m[3][3] - m.m[2][3] * m.m[3][1]) - m.m[0][1] * (m.m[2][0] * m.m[3][3] - m.m[2][3] * m.m[3][0]) + m.m[0][3] * (m.m[2][0] * m.m[3][1] - m.m[2][1] * m.m[3][0])) * determinantRecp;
	result.m[2][2] = (m.m[0][0] * (m.m[1][1] * m.m[3][3] - m.m[1][3] * m.m[3][1]) - m.m[0][1] * (m.m[1][0] * m.m[3][3] - m.m[1][3] * m.m[3][0]) + m.m[0][3] * (m.m[1][0] * m.m[3][1] - m.m[1][1] * m.m[3][0])) * determinantRecp;
	result.m[2][3] = -(m.m[0][0] * (m.m[1][1] * m.m[2][3] - m.m[1][3] * m.m[2][1]) - m.m[0][1] * (m.m[1][0] * m.m[2][3] - m.m[1][3] * m.m[2][0]) + m.m[0][3] * (m.m[1][0] * m.m[2][1] - m.m[1][1] * m.m[2][0])) * determinantRecp;

	result.m[3][0] = -(m.m[1][0] * (m.m[2][1] * m.m[3][2] - m.m[2][2] * m.m[3][1]) - m.m[1][1] * (m.m[2][0] * m.m[3][2] - m.m[2][2] * m.m[3][0]) + m.m[1][2] * (m.m[2][0] * m.m[3][1] - m.m[2][1] * m.m[3][0])) * determinantRecp;
	result.m[3][1] = (m.m[0][0] * (m.m[2][1] * m.m[3][2] - m.m[2][2] * m.m[3][1]) - m.m[0][1] * (m.m[2][0] * m.m[3][2] - m.m[2][2] * m.m[3][0]) + m.m[0][2] * (m.m[2][0] * m.m[3][1] - m.m[2][1] * m.m[3][0])) * determinantRecp;
	result.m[3][2] = -(m.m[0][0] * (m.m[1][1] * m.m[3][2] - m.m[1][2] * m.m[3][1]) - m.m[0][1] * (m.m[1][0] * m.m[3][2] - m.m[1][2] * m.m[3][0]) + m.m[0][2] * (m.m[1][0] * m.m[3][1] - m.m[1][1] * m.m[3][0])) * determinantRecp;
	result.m[3][3] = (m.m[0][0] * (m.m[1][1] * m.m[2][2] - m.m[1][2] * m.m[2][1]) - m.m[0][1] * (m.m[1][0] * m.m[2][2] - m.m[1][2] * m.m[2][0]) + m.m[0][2] * (m.m[1][0] * m.m[2][1] - m.m[1][1] * m.m[2][0])) * determinantRecp;

	return result;
}

Vector3 DebugCamera::Transform(const Vector3& vector, const Matrix4x4& matrix)
{
	Vector3 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
	if (w != 0.0f) {
		result.x /= w; result.y /= w; result.z /= w;
	}
	return result;
}

Matrix4x4 MakeRotateXMatrix(float radian)
{
	float c = std::cos(radian);
	float s = std::sin(radian);
	return Matrix4x4{ {
		{1.0f, 0.0f, 0.0f, 0.0f},
		{0.0f,    c,    s, 0.0f},
		{0.0f,   -s,    c, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}
	} };
}

Matrix4x4 MakeRotateYMatrix(float radian)
{
	float c = std::cos(radian);
	float s = std::sin(radian);
	return Matrix4x4{ {
		{   c, 0.0f,   -s, 0.0f},
		{0.0f, 1.0f, 0.0f, 0.0f},
		{   s, 0.0f,    c, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}
	} };
}


Matrix4x4 MakeRotateZMatrix(float radian)
{
	float c = std::cos(radian);
	float s = std::sin(radian);
	return Matrix4x4{ {
		{   c,    s, 0.0f, 0.0f},
		{  -s,    c, 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}
	} };
}

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2)
{
	Matrix4x4 result{};
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			result.m[y][x] = m1.m[y][0] * m2.m[0][x] + m1.m[y][1] * m2.m[1][x] + m1.m[y][2] * m2.m[2][x] + m1.m[y][3] * m2.m[3][x];
		}
	}
	return result;
};

Matrix4x4 MakeTranslateMatrix(const Vector3& translate)
{
	Matrix4x4 result = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		translate.x, translate.y, translate.z, 1.0f
	};
	return result;
};

void DebugCamera::Initialize()
{

}

void DebugCamera::Update(BYTE* key)
{
	//入力による移動や回転

	//前後移動
	if (key[DIK_W])//前移動の入力があったら
	{
		const float speed = 0.1f;//前進の速さ
		//移動ベクトルを角度分だけ回転
		Vector3 move = { 0.0f,0.0f,speed };
		Matrix4x4 rotMatX = MakeRotateXMatrix(move.x);
		Matrix4x4 rotMatY = MakeRotateYMatrix(move.y);
		Matrix4x4 rotMatZ = MakeRotateZMatrix(move.z);

		Matrix4x4 rotMat = Multiply(Multiply(rotMatX, rotMatY), rotMatZ);

		move = Transform(move, rotMat);

		//移動ベクトル文だけ座標を加算する
		translation_.z += move.z;
		translation_.x += move.x;
		translation_.y += move.y;
	}
	if (key[DIK_S])//前移動の入力があったら
	{
		const float speed = -0.1f;//前進の速さ
		//移動ベクトルを角度分だけ回転
		Vector3 move = { 0.0f,0.0f,speed };
		Matrix4x4 rotMatX = MakeRotateXMatrix(move.x);
		Matrix4x4 rotMatY = MakeRotateYMatrix(move.y);
		Matrix4x4 rotMatZ = MakeRotateZMatrix(move.z);

		Matrix4x4 rotMat = Multiply(Multiply(rotMatX, rotMatY), rotMatZ);

		move = Transform(move, rotMat);

		//移動ベクトル文だけ座標を加算する
		translation_.z += move.z;
		translation_.x += move.x;
		translation_.y += move.y;
	}
	if (key[DIK_A])//前移動の入力があったら
	{
		const float speed = -0.1f;//前進の速さ
		//移動ベクトルを角度分だけ回転
		Vector3 move = { speed,0.0f,0.0f };
		Matrix4x4 rotMatX = MakeRotateXMatrix(move.x);
		Matrix4x4 rotMatY = MakeRotateYMatrix(move.y);
		Matrix4x4 rotMatZ = MakeRotateZMatrix(move.z);

		Matrix4x4 rotMat = Multiply(Multiply(rotMatX, rotMatY), rotMatZ);

		move = Transform(move, rotMat);

		//移動ベクトル文だけ座標を加算する
		translation_.z += move.z;
		translation_.x += move.x;
		translation_.y += move.y;
	}

	if (key[DIK_D])//前移動の入力があったら
	{
		const float speed = 0.1f;//前進の速さ
		//移動ベクトルを角度分だけ回転
		Vector3 move = { speed,0.0f,0.0f };
		Matrix4x4 rotMatX = MakeRotateXMatrix(move.x);
		Matrix4x4 rotMatY = MakeRotateYMatrix(move.y);
		Matrix4x4 rotMatZ = MakeRotateZMatrix(move.z);

		Matrix4x4 rotMat = Multiply(Multiply(rotMatX, rotMatY), rotMatZ);

		move = Transform(move, rotMat);

		//移動ベクトル文だけ座標を加算する
		translation_.z += move.z;
		translation_.x += move.x;
		translation_.y += move.y;
	}

	if (key[DIK_Q])//前移動の入力があったら
	{
		const float speed = -0.1f;//前進の速さ
		//移動ベクトルを角度分だけ回転
		Vector3 move = { 0.0f,speed,0.0f };
		Matrix4x4 rotMatX = MakeRotateXMatrix(move.x);
		Matrix4x4 rotMatY = MakeRotateYMatrix(move.y);
		Matrix4x4 rotMatZ = MakeRotateZMatrix(move.z);

		Matrix4x4 rotMat = Multiply(Multiply(rotMatX, rotMatY), rotMatZ);

		move = Transform(move, rotMat);

		//移動ベクトル文だけ座標を加算する
		translation_.z += move.z;
		translation_.x += move.x;
		translation_.y += move.y;
	}
	if (key[DIK_E])//前移動の入力があったら
	{
		const float speed = 0.1f;//前進の速さ
		//移動ベクトルを角度分だけ回転
		Vector3 move = { 0.0f,speed,0.0f };
		Matrix4x4 rotMatX = MakeRotateXMatrix(move.x);
		Matrix4x4 rotMatY = MakeRotateYMatrix(move.y);
		Matrix4x4 rotMatZ = MakeRotateZMatrix(move.z);

		Matrix4x4 rotMat = Multiply(Multiply(rotMatX, rotMatY), rotMatZ);

		move = Transform(move, rotMat);

		//移動ベクトル文だけ座標を加算する
		translation_.z += move.z;
		translation_.x += move.x;
		translation_.y += move.y;
	}

	float rotX = 0;
	float rotY = 0;

	if (key[DIK_LEFT]) {
		rotY += -0.01f;
	}
	if (key[DIK_RIGHT]) {
		rotY += 0.01f;
	}
	if (key[DIK_UP]) {
		rotX += 0.01f;
	}
	if (key[DIK_DOWN]) {
		rotX += -0.01f;
	}

	//ビュー行列の更新

	//1.角度から回転行列を計算する
	Matrix4x4 matRotDelta = {
	1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	0,0,0,1,
	};
	matRotDelta = Multiply(matRotDelta, MakeRotateXMatrix(rotX));
	matRotDelta = Multiply(matRotDelta, MakeRotateYMatrix(rotY));;
	matRot_ = Multiply(matRotDelta ,matRot_);
	//2.座標から平行移動行列を計算する
	Matrix4x4 translationMatrix = MakeTranslateMatrix(translation_);
	//3.回転行列と平行移動行列からワールド行列を計算する
	Matrix4x4 worldMatrix = Multiply(matRot_, translationMatrix);
	//4.ワールド行列の逆行列をビュー行列に代入する
	viewMatrix = Inverse(worldMatrix);


}