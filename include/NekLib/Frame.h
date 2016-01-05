#ifndef NEKLIB_FRAME_H
#define NEKLIB_FRAME_H

#include <NekLib/Impl/Window.h>
#include <NekLib/Impl/Graphic.h>

#ifdef _DEBUG
#pragma comment(lib, "NekLib_d.lib")
#else
#pragma comment(lib, "NekLib.lib")
#endif

namespace NekLib
{

//Frame�N���X��Window�N���X��Graphic�N���X�Ƃ���2�̃T�u�V�X�e���ւ̃t�@�T�[�h�p�^�[���Ƃ��ē����܂�
class Frame
{
public:

	//�C���X�^���X�𓾂�
	
	static Frame* Get();

	//������
	bool Init(const char* title, int width, int height, bool consoleFlag, bool isWindowed,
		int x, int y, const HICON hIcon, const HCURSOR hCursor);
	bool Init(const char* title, int width, int height, bool consoleFlag, bool isWindowed);

	//�E�C���h�E�A�f�o�C�X�����
	bool Create();

	//���C�����[�v
	template<typename Type>
	bool Run(Type* pType);
	bool Run(int (*Update)(), int (*Draw)());

	//�A�N�Z�T�[

	int Width() const;
	int Height() const;

private:
	//�����R�s�[������֎~����
	Frame();
	Frame(const Frame& rhs);
	Frame& operator=(const Frame& rhs);
	~Frame(){}


	//�`��I�n
	bool Begin();
	bool End();

	Window* pWindow;	//Window�N���X�̃|�C���^
	Graphic* pGraphic;	//Graphic�N���X�̃|�C���^
};

template <typename Type>
bool Frame::Run(Type* pType)
{
	while(pWindow->ProcessMessage())
	{
		//�X�V
		pType->Update();

		Begin();

		//�`��
		pType->Draw();

		End();
	}

	return true;
}

}//namespace NekLib

#endif