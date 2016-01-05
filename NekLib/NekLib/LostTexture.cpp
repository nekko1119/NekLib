#include <LostTexture.h>
#include <Frame.h>
#include <Common.h>

#include <d3d9.h>
#include <d3dx9.h>

using namespace NekLib;

struct LostTexture::Impl
{
	ComPtr<IDirect3DTexture9> pD3DT;//テクスチャ
	D3DSURFACE_DESC D3DSD;			//テクスチャ情報
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

	//テクスチャの作成
	if(FAILED(D3DXCreateTexture(
		Frame::GetInstance()->GetDevice(),	//デバイス
		m_pImpl->D3DSD.Width,				//幅
		m_pImpl->D3DSD.Height,				//高さ
		0U,									//？
		m_pImpl->D3DSD.Usage,				//？
		m_pImpl->D3DSD.Format,				//フォーマット
		m_pImpl->D3DSD.Pool,				//？
		temp.ToCreate()						//テクスチャCOMのダブルポインタ
		)))
	{
		EXIT("テクスチャの作成に失敗しました");
	}

	m_pImpl->pD3DT.Swap(temp);
}

size_t LostTexture::GetCount() const
{
	return m_pImpl->pD3DT.GetCount();
}