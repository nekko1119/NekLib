#ifndef _NEKLIB_FRAMEIMPL_H_
#define _NEKLIB_FRAMEIMPL_H_

#include <NekLib/Frame.h>
#include <exception>
#include <tchar.h>
using namespace NekLib;

Frame::Frame() :
pWindow(nullptr),
	pGraphic(nullptr)
{
	pWindow = Window::Get();
	pGraphic = Graphic::Get();
}

Frame* Frame::Get()
{
	static Frame instance;
	return &instance;
}

bool Frame::Init(const char* title, int width, int height, bool consoleFlag, bool isWindowed)
{
	pWindow = Window::Get();
	pGraphic = Graphic::Get();

	//Window
	try
	{
		if(!pWindow->Init(title, width, height, consoleFlag))
		{
			return false;
		}
	}
	//例外（引数が不正の値）が飛んできたらデフォ値を入れて続行
	catch(std::exception& e)
	{
		MessageBox(nullptr, _T(e.what()), _T("Frame::Create"), MB_OK | MB_ICONEXCLAMATION);
		if(!pWindow->Init("NekLib", 640, 480, consoleFlag))
		{
			return false;
		}
	}

	//Graphic
	try
	{
		if(!pGraphic->Init(pWindow, isWindowed))
		{
			return false;
		}
	}
	catch(std::exception& e)
	{
		MessageBox(nullptr, _T(e.what()), _T("Frame::Create"), MB_OK | MB_ICONEXCLAMATION);
	}

	return true;
}

bool Frame::Init(const char* title, int width, int height, bool consoleFlag, bool isWindowed,
	int x, int y, const HICON hIcon, const HCURSOR hCursor)
{
	pWindow = Window::Get();
	pGraphic = Graphic::Get();

	//Window
	try
	{
		if(!pWindow->Init(title, width, height, consoleFlag, x, y, hIcon, hCursor))
		{
			return false;
		}
	}
	//例外（引数が不正の値）が飛んできたらデフォ値を入れて続行
	catch(std::exception& e)
	{
		MessageBox(nullptr, _T(e.what()), _T("Frame::Create"), MB_OK | MB_ICONEXCLAMATION);
		if(!pWindow->Init("NekLib", 640, 480, consoleFlag))
		{
			return false;
		}
	}

	//Graphic
	try
	{
		if(!pGraphic->Init(pWindow, isWindowed))
		{
			return false;
		}
	}
	catch(std::exception& e)
	{
		MessageBox(nullptr, _T(e.what()), _T("Frame::Create"), MB_OK | MB_ICONEXCLAMATION);
	}

	return true;
}
bool Frame::Create()
{
	try
	{
		if(!pWindow->Create())
		{
			return false;
		}
	}
	catch(std::exception& e)
	{
		MessageBox(nullptr, _T(e.what()), _T("Frame::Create"), MB_OK | MB_ICONEXCLAMATION);
		throw;
	}

	try
	{
		if(!pGraphic->Create())
		{
			return false;
		}
	}
	catch(std::exception& e)
	{
		MessageBox(nullptr, _T(e.what()), _T("Frame::Create"), MB_OK | MB_ICONEXCLAMATION);
		throw;
	}

	return true;
}

bool Frame::Run(int (*Update)(), int (*Draw)())
{
	while(pWindow->ProcessMessage())
	{
		//更新
		Update();

		Begin();

		//描画
		Draw();

		End();
	}

	return true;
}

int Frame::Width() const
{
	return pWindow->Width();
}

int Frame::Height() const
{
	return pWindow->Height();
}

bool Frame::Begin()
{
	//描画開始
	pGraphic->Device()->Clear(
		0U,									//第2引数の矩形の数。NULLの場合は0を入れる
		nullptr,							//クリアする範囲。NULLだと画面全体になる
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,	//何をクリアするか。画面とZバッファ
		D3DCOLOR_XRGB(100, 149, 237),		//画面をクリアする時に使う色
		1.0f,								//Zバッファをクリアする時に使う値
		0U									//ステンシルバッファをクリアする時に使う値
		);

	pGraphic->Device()->BeginScene();

	return true;
}

bool Frame::End()
{
		pGraphic->Device()->EndScene();
	//描画終了

	//裏画面から表画面に移す。デバイスロストしているかどうかも調べる
	if(pGraphic->Device()->Present(
		nullptr,	//スワップする裏画面の範囲
		nullptr,	//スワップされる表画面の範囲
		nullptr,	//対象のウインドウ。NULLだとD3DPRESENT_PARAMETERS::hDeviceWindowになる
		nullptr		//NULLを指定しなければならない
		) == D3DERR_DEVICELOST)
	{
		if(pGraphic->Device()->TestCooperativeLevel() != D3DERR_DEVICELOST)
		{
			pGraphic->Device()->Reset(pGraphic->D3DPP());
			pGraphic->IsLostDevice(true);
		}
		else if(pGraphic->IsLostDevice())
		{
			pGraphic->IsLostDevice(false);
		}
	}

	return true;
}
#endif