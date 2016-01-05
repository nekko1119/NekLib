#include <LostVertexBuffer.h>
#include <Frame.h>
#include <Common.h>

#include <d3d9.h>
#include <d3dx9.h>

using namespace NekLib;

struct LostVertexBuffer::Impl
{
	ComPtr<IDirect3DVertexBuffer9> pD3DVB;	//頂点バッファ
	D3DVERTEXBUFFER_DESC D3DVBD;			//頂点バッファ情報
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
		4 * sizeof(m_pImpl->D3DVBD.Size),	//使用するメモリ（1頂点のサイズ*4つ）
		m_pImpl->D3DVBD.Usage,				//頂点バッファの使い方。書き込み専用
		m_pImpl->D3DVBD.FVF,				//頂点バッファのフォーマット
		m_pImpl->D3DVBD.Pool,				//使用メモリの管理方法
		temp.ToCreate(),					//頂点バッファを管理するCOMのポインタのポインタ
		NULL								//未使用。必ずNULLを指定する
		)))
	{
		EXIT("頂点バッファの作成に失敗しました");
	}

	m_pImpl->pD3DVB.Swap(temp);
}

size_t LostVertexBuffer::GetCount() const
{
	return m_pImpl->pD3DVB.GetCount();
}