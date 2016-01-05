#ifndef NEKLIB_COMPTR_H
#define NEKLIB_COMPTR_H

#include "Impl\comptr.h"

namespace NekLib
{

template<class T>
class ComPtr
{
public:
	ComPtr();
	explicit ComPtr(T* ptr);
	ComPtr(const ComPtr<T>& rhs);
	~ComPtr();
	ComPtr<T>& operator=(const ComPtr<T>& rhs);
	ComPtr<T>& operator=(T* rhs);
	void Swap(ComPtr<T>& rhs);

	//メンバ関数
	T* Get();
	T** ToCreate();
	size_t GetCount() const;

	//演算子のオーバーロード
	T& operator*();
	T* operator->();
	bool operator==(const ComPtr<T>& rhs) const;
	bool operator!=(const ComPtr<T>& rhs) const;
	bool operator!();

private:
	IKD::Com_ptr<T> m_comPtr;
};

template<class T>
ComPtr<T>::ComPtr() : m_comPtr(NULL)
{
}

template<class T>
ComPtr<T>::ComPtr(T* ptr) : m_comPtr(ptr)
{
}

template<class T>
ComPtr<T>::ComPtr(const ComPtr<T>& rhs) : m_comPtr(rhs.m_comPtr)
{
}

template<class T>
ComPtr<T>::~ComPtr()
{
}

template<class T>
ComPtr<T>& ComPtr<T>::operator=(const ComPtr<T>& rhs)
{
	m_comPtr = rhs.m_comPtr;
	return *this;
}

template<class T>
ComPtr<T>& ComPtr<T>::operator=(T* rhs)
{
	m_comPtr = rhs;
	return *this;
}

template<class T>
T* ComPtr<T>::Get()
{
	return m_comPtr.GetPtr();
}

template<class T>
void ComPtr<T>::Swap(ComPtr<T>& rhs)
{
	m_comPtr.Swap(rhs.m_comPtr);
}


template<class T>
T** ComPtr<T>::ToCreate()
{
	return m_comPtr.ToCreator();
}

template<class T>
size_t ComPtr<T>::GetCount() const
{
	return m_comPtr.GetRefCnt();
}

template<class T>
T& ComPtr<T>::operator*()
{
	return *(m_comPtr.GetPtr());
}

template<class T>
T* ComPtr<T>::operator->()
{
	return m_comPtr.GetPtr();
}

template<class T>
bool ComPtr<T>::operator==(const ComPtr<T>& rhs) const
{
	return this->m_comPtr == rhs.m_comPtr;
}

template<class T>
bool ComPtr<T>::operator!=(const ComPtr<T>& rhs) const
{
	return this->m_comPtr != rhs.m_comPtr;
}

template<class T>
bool ComPtr<T>::operator!()
{
	return !m_comPtr;
}

}//namespace NekLib

#endif