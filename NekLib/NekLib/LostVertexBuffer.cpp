#include <LostVertexBuffer.h>
#include <Frame.h>
#include <Common.h>

#include <d3d9.h>
#include <d3dx9.h>

using namespace NekLib;

struct LostVertexBuffer::Impl
{
	ComPtr<IDirect3DVertexBuffer9> pD3DVB;	//���_�o�b�t�@
	D3DVERTEXBUFFER_DESC D3DVBD;			//���_�o�b�t�@���
};

LostVertexBuffer::LostVertexBuffer() : m_pImpl(new Impl)
{
}

LostVertexBuffer::~LostVertexBuffer()
{
}

void LostVertexBuffer::Register(ComPtr<IDirect3DVertexBuffer9> pD3DVB)
{
	pD3DVB->GetDesc(&(m_pImpl->D3DVBD));
	m_pImpl->pD3DVB = pD3DVB;
}

void LostVertexBuffer::Backup()
{
	if(m_pImpl->pD3DVB.Get())
	{
		ComPtr<IDirect3DVertexBuffer9> temp;
		m_pImpl->pD3DVB.Swap(temp);
	}
}

void LostVertexBuffer::Recover()
{
	ComPtr<IDirect3DVertexBuffer9> temp;

	if(FAILED(Frame::GetInstance()->GetDevice()->CreateVertexBuffer
		(
		4 * sizeof(m_pImpl->D3DVBD.Size),	//�g�p���郁�����i1���_�̃T�C�Y*4�j
		m_pImpl->D3DVBD.Usage,				//���_�o�b�t�@�̎g�����B�������ݐ�p
		m_pImpl->D3DVBD.FVF,				//���_�o�b�t�@�̃t�H�[�}�b�g
		m_pImpl->D3DVBD.Pool,				//�g�p�������̊Ǘ����@
		temp.ToCreate(),					//���_�o�b�t�@���Ǘ�����COM�̃|�C���^�̃|�C���^
		NULL								//���g�p�B�K��NULL���w�肷��
		)))
	{
		EXIT("���_�o�b�t�@�̍쐬�Ɏ��s���܂���");
	}

	m_pImpl->pD3DVB.Swap(temp);
}

size_t LostVertexBuffer::GetCount() const
{
	return m_pImpl->pD3DVB.GetCount();
}