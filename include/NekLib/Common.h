#ifndef NEKLIB_COMMON_H
#define NEKLIB_COMMON_H

#include <sstream>
#include <tchar.h>
#include <iostream>

#include <NekLib/Frame.h>

namespace NekLib
{

//���S��delete
template <typename Type>
inline void Delete(Type*& ptr)
{
	if(ptr)
	{
		delete ptr;
		ptr = nullptr;
	}
}

//���S��delete[]
template <typename Type>
inline void DeleteArray(Type*& ptr)
{
	if(ptr)
	{
		delete[] ptr;
		ptr = nullptr;
	}
}

//���S��Release
template <typename Type>
inline void Release(Type*& ptr)
{
	if(ptr)
	{
		ptr->Release();
		ptr = nullptr;
	}
}

//�G���[�I��
#define EXIT(msg) {int line = __LINE__;const char* file = __FILE__;\
	std::ostringstream oss;\
	oss << msg << '\n' << "file : " << file << '\n' << "line : " << line << std::endl;\
	MessageBox(nullptr, _T(oss.str().c_str()), "Error", MB_OK | MB_ICONEXCLAMATION);\
	exit(1);\
}

}//namespace NekLib

#endif