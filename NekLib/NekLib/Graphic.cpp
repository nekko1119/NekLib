#include <NekLib/Impl/Graphic.h>
#include <NekLib/Impl/Window.h>
#include <NekLib/Common.h>
#include <stdexcept>
using namespace NekLib;

#pragma comment(lib, "d3d9.lib")

struct Graphic::Impl
{
	ComPtr<IDirect3D9> pD3D;			//Direct3D�C���^�[�t�F�C�X
	ComPtr<IDirect3DDevice9> pD3DDevice;//�f�o�C�X�B�r�f�I�J�[�h�𒊏ۉ���������
	D3DPRESENT_PARAMETERS d3dpp;		//�o�͉�ʂ̏���Z�߂��\����
	Window* pWindow;					//�E�C���h�E�N���X
	bool isLostDevice;					//�f�o�C�X���X�g���Ă��邩�ǂ���

	Impl();
};

Graphic::Impl::Impl() :
pD3D(nullptr),
	pD3DDevice(nullptr),
	pWindow(nullptr),
	isLostDevice(false)
{
}

Graphic* Graphic::Get()
{
	static Graphic instance;
	return &instance;
}

Graphic::Graphic() : m_pImpl(new Impl())
{
}

bool Graphic::Init(Window* pWindow, bool isWindowed)
{
	//Direct 3D�C���^�[�t�F�C�X�̍쐬
	m_pImpl->pD3D = Direct3DCreate9( D3D_SDK_VERSION );

	//�쐬�Ɏ��s��������O�𓊂���
	if(!m_pImpl->pD3D)
	{
		throw std::runtime_error("Direct3D9�̍쐬�Ɏ��s���܂���");
	}

	m_pImpl->pWindow = pWindow;


	//�f�B�X�v���C�̏����擾

	D3DDISPLAYMODE d3ddm;
	
	if(FAILED(m_pImpl->pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return false;
	}
		
	m_pImpl->d3dpp.BackBufferWidth = m_pImpl->pWindow->Width();//����ʂ̕�	
	m_pImpl->d3dpp.BackBufferHeight = m_pImpl->pWindow->Height();//����ʂ̍���
	m_pImpl->d3dpp.BackBufferFormat = d3ddm.Format;//�F�[�x�i�\���ł���F���j
	m_pImpl->d3dpp.BackBufferCount = 1;//����ʂ̖����B���ʂ�1��
	m_pImpl->d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;//�����������قǁA�W���M������A���`�G�C���A�X���|����A����Y��ȉ摜�ɂȂ�B����������̓r�f�I�J�[�h�ˑ��Ȃ̂ŁA�ǂ̃r�f�I�J�[�h�ł������悤��D3DMULTISAMPLE_NONE���w�肷��
	m_pImpl->d3dpp.MultiSampleQuality = 0U;//MultiSampleType�̎��BNONE�̏ꍇ��0���w��
	m_pImpl->d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;//����ʂƕ\��ʂ̐ؑ֕��@�B���̒萔�̓f�B�X�v���C�h���C�o�������Ŕ��ʂ��Ă����Ƃ����Ӗ�
	m_pImpl->d3dpp.hDeviceWindow = nullptr;//�Ώۂ̃E�C���h�E���w��BNULL�Ō��݂̃t�H�[�J�X�E�C���h�E�ɂȂ�B
	m_pImpl->d3dpp.EnableAutoDepthStencil = TRUE;//�[�x�X�e���V���X�o�b�t�@�̗L���B����ɂ�Z�o�b�t�@�i���s���j���܂܂�邽�߁A3D�����Ȃ�K�v�B���Ȃ��Ȃ�i2D�Ȃ�jFALSE�ł�����
	m_pImpl->d3dpp.AutoDepthStencilFormat = D3DFMT_D16;//�[�x�X�e���V���o�t�@�̐ݒ�BZ�o�b�t�@�ɉ��r�b�g�g����
	m_pImpl->d3dpp.Flags = 0U;//����ʂ���\��ʂɓ]������Ƃ��̋@�\�̃I�v�V�����B�������Ȃ��̂�0���w��
	m_pImpl->d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;//���\�؂�ւ��̃^�C�~���O���w��B���̒萔�́u�`�悪�I����Ă���؂�ւ���v�Ƃ����Ӗ�

	//�E�C���h�E���t���X�N���[�����ŕ���
	if(isWindowed)
	{
		m_pImpl->d3dpp.Windowed = TRUE;//�E�C���h�E�ŋN��
		m_pImpl->d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_INTERVAL_DEFAULT;//�t���X�N���[���̎��̃��t���b�V�����[�g���w��B�`�悪�I����Ă���
	}
	else
	{
		m_pImpl->d3dpp.Windowed = FALSE;
		m_pImpl->d3dpp.FullScreen_RefreshRateInHz = d3ddm.RefreshRate;//�t���X�N���[���̎��̃��t���b�V�����[�g���w��B�f�B�X�v���C�Ɠ����^�C�~���O��
	}

	return true;
}

bool Graphic::Create()
{
	//�f�o�C�X�̍쐬�B�Ō�܂Ŏ��s�������O�𓊂���
	//�`��A�v�Z���Ƀn�[�h�E�F�A
	if(FAILED(m_pImpl->pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		m_pImpl->pWindow->HWnd(),
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&(m_pImpl->d3dpp),
		m_pImpl->pD3DDevice.ToCreate()
		)))
	{
		if(FAILED(m_pImpl->pD3D->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			m_pImpl->pWindow->HWnd(),
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&(m_pImpl->d3dpp),
			m_pImpl->pD3DDevice.ToCreate()
			)))
		{
			if(FAILED(m_pImpl->pD3D->CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				m_pImpl->pWindow->HWnd(),
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&(m_pImpl->d3dpp),
				m_pImpl->pD3DDevice.ToCreate()
				)))
			{
				throw std::runtime_error("Graphic::Create��Direct3DDevice9�̍쐬�Ɏ��s���܂���");
			}
		}
	}

	//TODO:�N���C�A���g���ݒ�ł���悤�ɂ���i���C�e�B���O�Ƃ����j
	//Z�o�b�t�@��L���ɂ���
	m_pImpl->pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	//�E�C���h�E�̕\��
	ShowWindow(m_pImpl->pWindow->HWnd(), SW_SHOW);//�\��
	ValidateRect(m_pImpl->pWindow->HWnd(), nullptr);//WM_PAINT�𓊂��Ȃ��悤�ɂ���

	return true;
}

ComPtr<IDirect3DDevice9> Graphic::Device() const
{
	return m_pImpl->pD3DDevice;
}

D3DPRESENT_PARAMETERS* Graphic::D3DPP() const
{
	return &m_pImpl->d3dpp;
}

bool Graphic::IsLostDevice() const
{
	return m_pImpl->isLostDevice;
}

void Graphic::IsLostDevice(bool flag)
{
	m_pImpl->isLostDevice = flag;
}