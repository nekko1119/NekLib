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

	//インスタンスを作成

	static Graphic* Get();

	//初期化
	
	bool Init(Window* pWindow, bool isWindowed);

	//デバイスを作成

	bool Create();

	//アクセサー

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