#ifndef NEKLIB_MATRIX_H
#define NEKLIB_MATRIX_H

#include <d3dx9math.h>

namespace NekLib
{

class Vector;

class Matrix
{
public:
	Matrix();
	Matrix(
		float m11, float m12, float m13, float m14,
		float m21, float m22, float m23, float m24,
		float m31, float m32, float m33, float m34,
		float m41, float m42, float m43, float m44
		);
	Matrix(const Matrix& rhs);
	Matrix(Matrix&& rhs);
	Matrix& operator=(const Matrix& rhs);
	Matrix& operator=(Matrix&& rhs);
	bool operator!=(const Matrix& rhs) const;
	bool operator==(const Matrix& rhs) const;
	void Swap(Matrix& rhs);

	//キャスト

	operator D3DMATRIX();

	//メンバ関数
	
	Matrix Rotate(const Vector& axis, const float rad);
	Matrix RotateX(const float rad);
	Matrix RotateY(const float rad);
	Matrix RotateZ(const float rad);
	Matrix Scale(const float rate);
	Matrix Scale(const float rateX, const float rateY, const float rateZ);
	Matrix Scale(const Vector& rate);
	Matrix Translate(const float moveX, const float moveY, const float moveZ);
	Matrix Translate(const Vector& move);

	//静的メンバ

	static Matrix Identity;

	//要素

	float* m;
};

}//namespace NekLib
#endif