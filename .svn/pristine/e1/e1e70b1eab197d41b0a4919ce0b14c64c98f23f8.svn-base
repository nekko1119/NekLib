#ifndef NEKLIB_MATHHELPER_H
#define NEKLIB_MATHHELPER_H

#include <cmath>
#include <memory>
#include <boost/scoped_array.hpp>

namespace NekLib
{

class MathHelper
{
public:

	static MathHelper* Get();

	inline float Sin(float rad);
	inline float Cos(float rad);

	static const float PI;

private:
	MathHelper();
	MathHelper(const MathHelper& rhs);
	MathHelper& operator=(const MathHelper& rhs);
	~MathHelper(){}

	boost::scoped_array<float> sinTable;
	boost::scoped_array<float> cosTable;
	boost::scoped_array<float> tanTable;
};

const float MathHelper::PI = 3.141592653589f;

MathHelper::MathHelper() : sinTable(new float[360]), cosTable(new float[360]), tanTable(new float[360])
{
	for(int i = 0; i < 360; i++)
	{
		sinTable[i] = std::sinf(i*PI/180);
	}
}

MathHelper* MathHelper::Get()
{
	static MathHelper instance;
	return &instance;
}

}//namespace NekLib

#endif