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
	//��O�i�������s���̒l�j�����ł�����f�t�H�l�����đ��s
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
	//��O�i�������s���̒l�j�����ł�����f�t�H�l�����đ��s
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
		//�X�V
		Update();

		Begin();

		//�`��
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
	//�`��J�n
	pGraphic->Device()->Clear(
		0U,									//��2�����̋�`�̐��BNULL�̏ꍇ��0������
		nullptr,							//�N���A����͈́BNULL���Ɖ�ʑS�̂ɂȂ�
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,	//�����N���A���邩�B��ʂ�Z�o�b�t�@
		D3DCOLOR_XRGB(100, 149, 237),		//��ʂ��N���A���鎞�Ɏg���F
		1.0f,								//Z�o�b�t�@���N���A���鎞�Ɏg���l
		0U									//�X�e���V���o�b�t�@���N���A���鎞�Ɏg���l
		);

	pGraphic->Device()->BeginScene();

	return true;
}

bool Frame::End()
{
		pGraphic->Device()->EndScene();
	//�`��I��

	//����ʂ���\��ʂɈڂ��B�f�o�C�X���X�g���Ă��邩�ǂ��������ׂ�
	if(pGraphic->Device()->Present(
		nullptr,	//�X���b�v���闠��ʂ͈̔�
		nullptr,	//�X���b�v�����\��ʂ͈̔�
		nullptr,	//�Ώۂ̃E�C���h�E�BNULL����D3DPRESENT_PARAMETERS::hDeviceWindow�ɂȂ�
		nullptr		//NULL���w�肵�Ȃ���΂Ȃ�Ȃ�
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