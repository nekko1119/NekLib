#include <stdexcept>
#include <NekLib/Impl/Window.h>
#include <tchar.h>
using namespace NekLib;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

bool g_isActive = false;//�E�C���h�E���A�N�e�B�u���ǂ���

struct Window::Impl
{
	bool consoleFlag;	//�R���\�[����ʂ��쐬���邩�ǂ����̃t���O
	FILE* consoleFp;	//�R���\�[����ʂ̃t�@�C���|�C���^
	int width;			//�E�C���h�E�̃N���C�A���g�̈�̕�
	int height;			//�E�C���h�E�̃N���C�A���g�̈�̍���
	int wndWidth;		//�E�C���h�E�̕�
	int wndHeight;		//�E�C���h�E�̍���
	int x;				//�E�C���h�E��x���W
	int y;				//�E�C���h�E��y���W
	HWND hWnd;			//�E�C���h�E�n���h��
	WNDCLASS wc;		//�E�C���h�E�N���X
	MSG msg;			//�E�C���h�E���b�Z�[�W
	DWORD style;		//�E�C���h�E�̃X�^�C��

	Impl();
	~Impl();
};

Window::Impl::Impl() :
	consoleFlag(false),
	consoleFp(nullptr),
	width(0),
	height(0),
	wndWidth(0),
	wndHeight(0),
	x(0U),
	y(0U),
	hWnd(nullptr),
	style(0U)
{
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.lpszClassName = _T("NekLib");
}

Window::Impl::~Impl()
{
	UnregisterClass(wc.lpszClassName, nullptr);

	if(consoleFlag)
	{
		fclose(consoleFp);
		FreeConsole();
	}
}

Window* Window::Get()
{
	static Window instance;
	return &instance;
}

Window::Window() : m_pImpl(new Impl())
{
}

bool Window::Init(const char* title, int width, int height, bool consoleFlag)
{
	//�l���s���Ȃ��O�𓊂���
	if(!title || width <= 0 || height <= 0)
	{
		throw std::invalid_argument("Window::Init�̈����̒l������������܂���");
	}

	m_pImpl->width = width;
	m_pImpl->height = height;
	m_pImpl->wc.lpszClassName = _T(title);
	m_pImpl->consoleFlag = consoleFlag;

	//WNDCLASS�̐ݒ�

	m_pImpl->wc.style = 0U;					//�E�C���h�E�X�^�C���BDirectGraphics�Őݒ肷��̂ŉ������Ȃ�
	m_pImpl->wc.lpfnWndProc = WindowProc;	//���̃E�C���h�E�̃��b�Z�[�W���Ǘ�����֐����w��
	m_pImpl->wc.cbClsExtra = 0;				//�\���̂̒ǉ��e�ʂ��o�C�g�Ŏw��B���ʂ�0
	m_pImpl->wc.cbWndExtra = 0;				//�E�C���h�E�̒ǉ��e�ʂ��o�C�g�Ŏw��B���ʂ�0
	m_pImpl->wc.hInstance = nullptr;		//�A�v���P�[�V�����̃C���X�^���X�n���h���BNULL���w�肷��ƌ��݂̃A�v���P�[�V�������擾����B
	m_pImpl->wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);//�E�C���h�E�̔w�i�F�B�ǂ���DirectX�ŏ㏑�������
	m_pImpl->wc.lpszMenuName = nullptr;		//�E�C���h�E�̃��j���[�̖��O�B���j���[�͑��݂��Ȃ��̂�NULL

	//�`�`�`�`�`�����݂̃E�C���h�E�̑傫�������߂�`�`�`�`�`

	RECT rect;//�E�C���h�E�̑傫���i�N���C�A���g�̈�{���j

	//�N���C�A���g�̈�̑傫����ݒ肷��
	SetRect(&rect, 0, 0, width, height);

	//�E�C���h�E�̃X�^�C��
	//�ʏ�̃E�C���h�E����T�C�Y�ύX�ƍő剻���Ȃ���������
	m_pImpl->style = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX;

	//�����݂̃E�C���h�E�̑傫�������߂āArect�ɓ����
	AdjustWindowRect(&rect, m_pImpl->style, FALSE);
	m_pImpl->wndWidth = rect.right - rect.left;
	m_pImpl->wndHeight = rect.bottom - rect.top;

	//�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`

	//�E�C���h�E�̍��W�i�f�B�X�v���C�̒����ɕ\������悤�ɂ���j
	m_pImpl->x = (GetSystemMetrics(SM_CXSCREEN) - (m_pImpl->wndWidth)) / 2;
	m_pImpl->y = (GetSystemMetrics(SM_CYSCREEN) - (m_pImpl->wndHeight)) / 2;

	return true;
}

bool Window::Init(const char* title, int width, int height, bool consoleFlag,
		int x, int y, const HICON hIcon, const HCURSOR hCursor)
{
	//�l���s���Ȃ��O�𓊂���
	if(!title || width <= 0 || height <= 0 || !hIcon || !hCursor)
	{
		throw std::invalid_argument("Window::Init�̈����̒l������������܂���");
	}

	m_pImpl->width = width;
	m_pImpl->height = height;
	m_pImpl->wc.lpszClassName = _T(title);
	m_pImpl->consoleFlag = consoleFlag;
	m_pImpl->x = x;
	m_pImpl->y = y;

	//WNDCLASS�̐ݒ�

	m_pImpl->wc.hIcon = hIcon;
	m_pImpl->wc.hCursor = hCursor;

	m_pImpl->wc.style = 0U;					//�E�C���h�E�X�^�C���BDirectGraphics�Őݒ肷��̂ŉ������Ȃ�
	m_pImpl->wc.lpfnWndProc = WindowProc;	//���̃E�C���h�E�̃��b�Z�[�W���Ǘ�����֐����w��
	m_pImpl->wc.cbClsExtra = 0;				//�\���̂̒ǉ��e�ʂ��o�C�g�Ŏw��B���ʂ�0
	m_pImpl->wc.cbWndExtra = 0;				//�E�C���h�E�̒ǉ��e�ʂ��o�C�g�Ŏw��B���ʂ�0
	m_pImpl->wc.hInstance = nullptr;		//�A�v���P�[�V�����̃C���X�^���X�n���h���BNULL���w�肷��ƌ��݂̃A�v���P�[�V�������擾����B
	m_pImpl->wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);//�E�C���h�E�̔w�i�F�B�ǂ���DirectX�ŏ㏑�������
	m_pImpl->wc.lpszMenuName = nullptr;		//�E�C���h�E�̃��j���[�̖��O�B���j���[�͑��݂��Ȃ��̂�NULL

	//�`�`�`�`�`�����݂̃E�C���h�E�̑傫�������߂�`�`�`�`�`

	RECT rect;//�E�C���h�E�̑傫���i�N���C�A���g�̈�{���j

	//�N���C�A���g�̈�̑傫����ݒ肷��
	SetRect(&rect, 0, 0, width, height);

	//�E�C���h�E�̃X�^�C��
	//�ʏ�̃E�C���h�E����T�C�Y�ύX�ƍő剻���Ȃ���������
	m_pImpl->style = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX;

	//�����݂̃E�C���h�E�̑傫�������߂āArect�ɓ����
	AdjustWindowRect(&rect, m_pImpl->style, FALSE);
	m_pImpl->wndWidth = rect.right - rect.left;
	m_pImpl->wndHeight = rect.bottom - rect.top;

	//�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`

	//�E�C���h�E�̍��W�i�f�B�X�v���C�̒����ɕ\������悤�ɂ���j
	m_pImpl->x = (GetSystemMetrics(SM_CXSCREEN) - (m_pImpl->wndWidth)) / 2;
	m_pImpl->y = (GetSystemMetrics(SM_CYSCREEN) - (m_pImpl->wndHeight)) / 2;

	return true;
}


bool Window::Create()
{
	//�R���\�[����ʂ̍쐬
	if(m_pImpl->consoleFlag)
	{
		AllocConsole();
		freopen_s(&(m_pImpl->consoleFp), "CONOUT$", "w", stdout);
		freopen_s(&(m_pImpl->consoleFp), "CONIN$", "w", stdin);
	}

	if(!RegisterClass(&(m_pImpl->wc)))
	{
		return false;
	}
	
	//WM_CREATE�𓊂���
	m_pImpl->hWnd = CreateWindow(
		m_pImpl->wc.lpszClassName,	//�E�C���h�E�̖��O
		m_pImpl->wc.lpszClassName,	//�E�C���h�E�̃^�C�g��
		m_pImpl->style,				//�E�C���h�E�̃X�^�C��
		m_pImpl->x,					//�E�C���h�E��x���W
		m_pImpl->y,					//�E�C���h�E��y���W
		m_pImpl->wndWidth,			//�E�C���h�E�̕�
		m_pImpl->wndHeight,			//�E�C���h�E�̍���
		nullptr,					//�e�E�C���h�E�B���݂��Ȃ��̂�NULL
		nullptr,					//�q�E�C���h�E�܂��̓��j���[�B���݂��Ȃ��̂�NULL
		nullptr,					//�A�v���P�[�V�����̃C���X�^���X�n���h���BNULL�Ō��݂̃��W���[�����w��
		nullptr						//WM_CREATE���b�Z�[�W��LPARAM�ɂȂ�\���̂̃|�C���^�B���ɖ����̂�NULL
		);

	if(!m_pImpl->hWnd)
	{
		return false;
	}

	return true;
}

bool Window::ProcessMessage()
{
	while(true)
	{
		if(PeekMessage(&(m_pImpl->msg), nullptr, 0U, 0U, PM_REMOVE))
		{
			if(m_pImpl->msg.message == WM_QUIT)
			{
				return false;
			}

			TranslateMessage(&(m_pImpl->msg));
			DispatchMessage(&(m_pImpl->msg));
		}
		else
		{
			return true;
		}
	}
}

std::string Window::Title() const
{
	return _T(m_pImpl->wc.lpszClassName);
}

int Window::Width() const
{
	return m_pImpl->width;
}

int Window::Height() const
{
	return m_pImpl->height;
}

bool Window::ConsoleFlag() const
{
	return m_pImpl->consoleFlag;
}

HWND Window::HWnd() const
{
	return m_pImpl->hWnd;
}

bool Window::IsActive() const
{
	return g_isActive;
}

//�E�C���h�E�v���V�[�W��
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch(msg)
	{
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
	case WM_KEYDOWN:
		{
			if(wp == VK_ESCAPE)
			{
				PostQuitMessage(0);
			}
			break;
		}
	case WM_ACTIVATE:
		{
			g_isActive = ((wp & 0xffff) != 0);
			break;
		}
	default:
		{
			return DefWindowProc(hWnd, msg, wp, lp);
			break;
		}
	}

	return 0;
}