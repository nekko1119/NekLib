#ifndef NEKLIB_GRAPHIC_H
#define NEKLIB_GRAPHIC_H

#include <memory> //for std::unique_ptr
#include <d3d9.h>
#include <NekLib/ComPtr.h>

namespace NekLib
{

class Window;

class Graphic
{
public:

	//�C���X�^���X���쐬

	static Graphic* Get();

	//������
	
	bool Init(Window* pWindow, bool isWindowed);

	//�f�o�C�X���쐬

	bool Create();

	//�A�N�Z�T�[

	ComPtr<IDirect3DDevice9> Device() const;
	D3DPRESENT_PARAMETERS* D3DPP() const;
	bool IsLostDevice() const;
	void IsLostDevice(bool flag);

private:
	Graphic();
	Graphic(const Graphic& rhs);
	Graphic& operator=(const Graphic& rhs);

	struct Impl;
	std::unique_ptr<Impl> m_pImpl;
};

}//namespace NekLib

#endif