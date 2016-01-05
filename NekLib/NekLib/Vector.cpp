#include <NekLib/Common.h>
#include <NekLib/Vector.h>
#include <algorithm>//for std::swap<T>
#include <stdexcept>//for std::logic_error
#include <d3d9types.h>
#include <d3dx9math.h>
using namespace NekLib;

#pragma comment(lib, "d3dx9.lib")

const Vector Vector::Zero(0.0f);
const Vector Vector::One(1.0f);

namespace std
{

//ì¡éÍâª
template<>
void swap<NekLib::Vector>(NekLib::Vector& lhs, NekLib::Vector& rhs)
{
	lhs.Swap(rhs);
}

}//namespace std;

Vector::Vector() : x(0.0f), y(0.0f), z(0.0f)
{
}

Vector::Vector(float x, float y, float z) : x(x), y(y), z(z)
{
}

Vector::Vector(float size) : x(size), y(size), z(size)
{
}

Vector::Vector(const Vector& rhs) : x(rhs.x), y(rhs.y), z(rhs.z)
{
}

Vector::Vector(Vector&& rhs) : x(rhs.x), y(rhs.y), z(rhs.z)
{
}

Vector& Vector::operator=(const Vector& rhs)
{
	using std::swap;

	Vector temp(rhs);
	swap(*this, temp);
	return *this;
}

Vector& Vector::operator=(Vector&& rhs)
{
	using std::swap;

	swap(*this, rhs);
	return *this;
}

bool Vector::operator!=(const Vector& rhs) const
{
	return !(x == rhs.x && y == rhs.y && z == rhs.z);
}

bool Vector::operator==(const Vector& rhs) const
{
	return (x == rhs.x && y == rhs.y && z == rhs.z);
}

void Vector::Swap(Vector& rhs)
{
	using std::swap;
	swap(x, rhs.x);
	swap(y, rhs.y);
	swap(z, rhs.z);
}

Vector::operator D3DVECTOR()
{
	D3DVECTOR ret = {x, y, z};
	return ret;
}

Vector::operator const D3DVECTOR() const
{
	const D3DVECTOR ret = {x, y, z};
	return ret;
}

Vector::operator D3DXVECTOR2()
{
	return D3DXVECTOR2(x, y);
}

Vector::operator const D3DXVECTOR2() const
{
	const D3DXVECTOR2 ret(x, y);
	return ret;
}

float Vector::Length() const
{
	return sqrt(x*x + y*y + z*z);
}

const Vector Vector::Cross(const Vector& rhs) const
{
	Vector ret;
	ret.x = y * rhs.z - z * rhs.y;
	ret.y = z * rhs.x - x * rhs.z;
	ret.z = x * rhs.y - y * rhs.x;
	return ret;
}

float Vector::Cross2D(const Vector& rhs) const
{
	return x*rhs.y - y*rhs.x;
}

float Vector::Dot(const Vector& rhs) const
{
	return x*rhs.x + y*rhs.y + z*rhs.z;
}

float Vector::Distance(const Vector& rhs) const
{
	return sqrt((rhs.x-x)*(rhs.x-x) + (rhs.y-y)*(rhs.y-y) + (rhs.z-z)*(rhs.z-z));
}

const Vector Vector::Normalize() const
{
	float len = Length();
	return Vector(x/len, y/len, z/len);
}

const Vector operator +(const Vector& lhs, const Vector& rhs)
{
	Vector ret;
	ret.x = lhs.x + rhs.x;
	ret.y = lhs.y + rhs.y;
	ret.z = lhs.z + rhs.z;
	return ret;
}

const Vector operator -(const Vector& lhs, const Vector& rhs)
{
	Vector ret;
	ret.x = lhs.x - rhs.x;
	ret.y = lhs.y - rhs.y;
	ret.z = lhs.z - rhs.z;
	return ret;
}

const Vector operator *(const Vector& lhs, const Vector& rhs)
{
	Vector ret;
	ret.x = lhs.x * rhs.x;
	ret.y = lhs.y * rhs.y;
	ret.z = lhs.z * rhs.z;
	return ret;
}

const Vector operator /(const Vector& lhs, const Vector& rhs)
{
	if(!rhs.x || !rhs.y || !rhs.z)
	{
		throw std::logic_error("Vector::operator/Å@0èúéZÇµÇÊÇ§Ç∆ÇµÇ‹ÇµÇΩ");
	}

	Vector ret;
	ret.x = lhs.x / rhs.x;
	ret.y = lhs.y / rhs.y;
	ret.z = lhs.z / rhs.z;
	return ret;
}