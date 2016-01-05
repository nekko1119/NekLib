#ifndef NEKLIB_VECTOR_H
#define NEKLIB_VECTOR_H

#include <ostream>//for std::ostream::operator<< overload

struct _D3DVECTOR;
typedef _D3DVECTOR D3DVECTOR;
struct D3DXVECTOR2;

namespace NekLib
{

class Vector
{
	friend std::ostream& operator<<(std::ostream& os, const Vector& v)
	{
		os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
		return os;
	}

public:
	Vector();
	Vector(float size);//not explicit
	Vector(float x, float y, float z);
	Vector(const Vector& rhs);
	Vector(Vector&& rhs);
	Vector& operator=(const Vector& rhs);
	Vector& operator=(Vector&& rhs);
	bool operator!=(const Vector& rhs) const;
	bool operator==(const Vector& rhs) const;
	void Swap(Vector& rhs);

	//キャスト

	operator D3DVECTOR();
	operator const D3DVECTOR() const;
	operator D3DXVECTOR2();
	operator const D3DXVECTOR2() const;
	
	//普通のメンバ関数

	float Length() const;							//スカラー
	const Vector Cross(const Vector& rhs) const;	//外積
	float Cross2D(const Vector& rhs) const;			//2次元の外積
	float Dot(const Vector& rhs) const;				//内積
	float Distance(const Vector& rhs) const;		//2つのベクトルの距離
	const Vector Normalize() const;					//正規化

	//静的メンバ
	
	static const Vector Zero;
	static const Vector One;
	static const Vector UnitX;
	static const Vector UnitY;
	static const Vector UnitZ;

	//要素

	float x;
	float y;
	float z;
};

const Vector operator+(const Vector& lhs, const Vector& rhs);
const Vector operator-(const Vector& lhs, const Vector& rhs);
const Vector operator*(const Vector& lhs, const Vector& rhs);
const Vector operator/(const Vector& lhs, const Vector& rhs);
const Vector operator+=(const Vector&lhs, const Vector& rhs);
const Vector operator-=(const Vector&lhs, const Vector& rhs);
const Vector operator*=(const Vector&lhs, const Vector& rhs);
const Vector operator/=(const Vector&lhs, const Vector& rhs);

}//namespace NekLib

#endif