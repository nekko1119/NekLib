#ifndef NEKLIB_WINDOW_H
#define NEKLIB_WINDOW_H

#include <string>
#include <memory>///for std::unique_ptr
#include <Windows.h>

namespace NekLib
{

class Window
{
public:

	//インスタンス取得

	static Window* Get();

	//初期化

	bool Init(const char* title, int width, int height, bool consoleFlag);
	bool Init(const char* title, int width, int height, bool consoleFlag,
		int x, int y, const HICON hIcon, const HCURSOR hCursor);

	//ウインドウを作成

	bool Create();

	//メッセージ処理

	bool ProcessMessage();

	//メンバ関数

	bool IsActive() const;


	//アクセサー

	std::string Title() const;

	int Width() const;

	int Height() const;

	bool ConsoleFlag() const;

	HWND HWnd() const;

private:
	Window();
	Window(const Window& rhs);
	Window& operator=(const Window& rhs);

	struct Impl;
	std::unique_ptr<Impl> m_pImpl;
};

}//namespace NekLib

#endif