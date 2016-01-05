#include <LostTexture.h>
#include <Frame.h>
#include <Common.h>

#include <d3d9.h>
#include <d3dx9.h>

using namespace NekLib;

struct LostTexture::Impl
{
	ComPtr<IDirect3DTexture9> pD3DT;//�e�N�X�`��
	D3DSURFACE_DESC D3DSD;			//�e�N�X�`�����
};

LostTexture::LostTexture() : m_pImpl(new Impl)
{
}

LostTexture::~LostTexture()
{
}

void LostTexture::Register(ComPtr<IDirect3DTexture9> pD3DT)
{
	pD3DT->GetLevelDesc(0U, &(m_pImpl->D3DSD));
	m_pImpl->pD3DT = pD3DT;
}

void LostTexture::Backup()
{
	if(m_pImpl->pD3DT.Get())
	{
		ComPtr<IDirect3DTexture9> temp;
		m_pImpl->pD3DT.Swap(temp);
	}
}

void LostTexture::Recover()
{
	ComPtr<IDirect3DTexture9> temp;

	//�e�N�X�`���̍쐬
	if(FAILED(D3DXCreateTexture(
		Frame::GetInstance()->GetDevice(),	//�f�o�C�X
		m_pImpl->D3DSD.Width,				//��
		m_pImpl->D3DSD.Height,				//����
		0U,									//�H
		m_pImpl->D3DSD.Usage,				//�H
		m_pImpl->D3DSD.Format,				//�t�H�[�}�b�g
		m_pImpl->D3DSD.Pool,				//�H
		temp.ToCreate()						//�e�N�X�`��COM�̃_�u���|�C���^
		)))
	{
		EXIT("�e�N�X�`���̍쐬�Ɏ��s���܂���");
	}

	m_pImpl->pD3DT.Swap(temp);
}

size_t LostTexture::GetCount() const
{
	return m_pImpl->pD3DT.GetCount();
}