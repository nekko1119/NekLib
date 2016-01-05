#include <NekLib/Common.h>
#include <NekLib/Image.h>
#include <NekLib/Frame.h>
#include <NekLib/ComPtr.h>

#include <algorithm>//for std::swap<T>
#include <cmath>
#include <boost/logic/tribool.hpp>

#include <d3d9.h>
#include <d3dx9.h>
using namespace NekLib;
using namespace boost::logic;

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

struct Image::Impl
{
	std::string name;						//ファイルの名前
	ComPtr<IDirect3DVertexBuffer9> pD3DVB;	//頂点バッファ
	ComPtr<IDirect3DTexture9> pD3DT;		//テクスチャ（画像）
	D3DXIMAGE_INFO D3DII;					//画像の情報
	BLENDMODE blendMode;					//ブレンドモード（このオブジェクトのモード）
	ALPHAMODE alphaMode;					//アルファモード
	BYTE alpha;								//アルファ値
	size_t transColor;						//透過色
	bool isCalledSetBlendMode;				//SetBlendModeを1回は呼んだかどうか
	size_t xNum;							//画像のx座標分割数
	size_t yNum;							//画像のy座標分割数
	float divideWidth;						//分割画像1枚の幅（数値）
	float divideHeight;						//分割画像1枚の高さ（数値）
	float divideWidthRate;					//分割画像1枚の幅（割合）
	float divideHeightRate;					//分夏画像1枚の高さ（割合）
	tribool isDivide;						//分割読み込みか、読み込みしたかどうか

	//頂点構造体
	struct Vertex2D
	{
		D3DXVECTOR3 pos;			//頂点の座標
		float rhw;					//変換済み座標
		DWORD dif;					//ディヒューズ色
		D3DXVECTOR2 tex;			//テクスチャの座標
	} v2D[4];				//テクスチャは長方形なので頂点は4つ

	static ComPtr<IDirect3DDevice9> pD3DDevice;//デバイス
	static const DWORD D3DFVF_2DVERTEX = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;//頂点フォーマット

	Impl();
	void Reset();							//デバイスロストした時に呼ばれる
	void SetBlendMode(BLENDMODE blendMode);	//ブレンドモードの設定
	void SetTextureAlphaStageState();		//テクスチャの透過設定
	void DrawInit();						//Draw関数での初期化処理
	void DrawInit(int num);					//Draw関数での初期化処理（分割画像）
	void Write();							//VRAMへの書き込み
	void DrawImpl();						//真の描画処理
};

ComPtr<IDirect3DDevice9> Image::Impl::pD3DDevice(NULL);

Image::Impl::Impl() :
	pD3DVB(NULL),
	pD3DT(NULL),
	blendMode(BLENDMODE_ALPHA),
	alphaMode(ALPHAMODE_DEFAULT),
	alpha(255),
	transColor(0U),
	isCalledSetBlendMode(false),
	xNum(0U),
	yNum(0U),
	divideWidth(0.0f),
	divideHeight(0.0f),
	divideWidthRate(0.0f),
	divideHeightRate(0.0f),
	isDivide(indeterminate)
{
	//デバイスの取得
	if(!pD3DDevice && Graphic::Get()->Device().Get())
	{
		pD3DDevice = Graphic::Get()->Device();
	}
}

void Image::Impl::Reset()
{
	SetBlendMode(blendMode);
}

void Image::Impl::SetBlendMode(BLENDMODE blendMode)
{
	//デバイスの取得
	if(!pD3DDevice && Graphic::Get()->Device().Get())
	{
		pD3DDevice = Graphic::Get()->Device();
	}
	else if(!pD3DDevice)
	{
		EXIT("FrameクラスのCreate関数を実行してから画像の読み込みをしてください");
	}

	switch(blendMode)
	{
	case BLENDMODE_NONE:
		{
			//アルファブレンドを無効にする
			pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

			break;
		}
	case BLENDMODE_ALPHA:
		{		
			//アルファブレンドを有効にする
			pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

			//ソースのα値を使う
			pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			break;
		}
	case BLENDMODE_ALPHA2:
		{
			//頂点のアルファ値を変える
			v2D[0].dif = v2D[1].dif = v2D[2].dif = v2D[3].dif = D3DCOLOR_RGBA(255, 255, 255, alpha);

			//アルファブレンドを有効にする
			pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

			//設定したのα値を使う
			pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			break;
		}
	case BLENDMODE_ADD:
		{
			//アルファブレンドを有効にする
			pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

			//ソースと元の画像を加算合成する
			pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			break;
		}
	case BLENDMODE_SUB:
		{
			//アルファブレンドを有効にする
			pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

			//ソースと元の画像を減算合成する
			pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
			pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			break;
		}
	case BLENDMODE_MUL:
		{
			//アルファブレンドを有効にする
			pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

			//ソースと元の画像を乗算合成する
			pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
			pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);

			break;
		}
	default:
		{
			break;
		}
	}

	isCalledSetBlendMode = true;
}

void Image::Impl::SetTextureAlphaStageState()
{	
	switch(alphaMode)
	{
	case ALPHAMODE_NONE:
		{
			//パイプラインの1つめの要素はテクスチャです、という設定
			pD3DDevice->SetTextureStageState(0U, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			//1つめの要素をそのまま出力する設定
			pD3DDevice->SetTextureStageState(0U, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

			break;
		}
	case ALPHAMODE_DEFAULT:
		{
			//カラーの1つめの要素はテクスチャです、という設定
			pD3DDevice->SetTextureStageState(0U, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			//1つめの要素をそのまま出力する設定
			pD3DDevice->SetTextureStageState(0U, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			//アルファチャンネル1つめの要素はテクスチャです
			pD3DDevice->SetTextureStageState(0U, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			//テクスチャのアルファ値をそのまま使います
			pD3DDevice->SetTextureStageState(0U, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

			break;
		}
	case ALPHAMODE_TRANS:
		{
			//頂点バッファのRGB使う
			pD3DDevice->SetTextureStageState(0U, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			pD3DDevice->SetTextureStageState(0U, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			pD3DDevice->SetTextureStageState(0U, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

			break;
		}
	default:
		{

			break;
		}
	}

	if(blendMode == BLENDMODE_MUL)
	{
		DWORD factor = 0xffffffff;//白
		pD3DDevice->SetRenderState(D3DRS_TEXTUREFACTOR, factor); //定数テクスチャを設定

		pD3DDevice->SetTextureStageState(1U, D3DTSS_COLORARG1, D3DTA_CURRENT);
		pD3DDevice->SetTextureStageState(1U, D3DTSS_COLORARG2, D3DTA_TFACTOR); //定数テクスチャを使用
		pD3DDevice->SetTextureStageState(1U, D3DTSS_COLOROP, D3DTOP_BLENDCURRENTALPHA);
		pD3DDevice->SetTextureStageState(1U, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
		pD3DDevice->SetTextureStageState(1U, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	}
	else
	{
		pD3DDevice->SetTextureStageState(1U, D3DTSS_COLORARG1, D3DTA_CURRENT);
		pD3DDevice->SetTextureStageState(1U, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		pD3DDevice->SetTextureStageState(1U, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
		pD3DDevice->SetTextureStageState(1U, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	}
}

void Image::Impl::DrawInit()
{
	//画像を読み込んでいないか、分割読み込みなら例外を投げる
	if(!pD3DT || isDivide)
	{
		throw std::logic_error("Image::Impl::DrawInitで、画像読み込んでいないか分割画像になっています");
	}

	//デバイスロストしていたら復帰する
	if(Graphic::Get()->IsLostDevice())
	{
		Reset();
	}

	//テクスチャステージの設定
	SetTextureAlphaStageState();

	//ブレンドモードの設定
	SetBlendMode(blendMode);

	//デバイスにテクスチャをセット
	pD3DDevice->SetTexture(0U, pD3DT.Get());
}

void Image::Impl::DrawInit(int num)
{
	//画像番号が分割数超えていたら例外を投げる
	if(static_cast<unsigned int>(num) >= xNum * yNum)
	{
		throw std::out_of_range("Image::Impl::Draw引数の画像番号が分割数を超えています");
	}

	//画像を読み込んでいないか、非分割読み込みなら例外を投げる
	if(!pD3DT || !isDivide)
	{
		throw std::logic_error("Image::Impl::Draw画像が読み込まれていないか非分割読み込みです");
	}

	//デバイスロストしていたら復帰する
	if(Graphic::Get()->IsLostDevice())
	{
		Reset();
	}

	//テクスチャステージの設定
	SetTextureAlphaStageState();

	//ブレンドモードの設定
	SetBlendMode(blendMode);

	//デバイスにテクスチャをセット
	pD3DDevice->SetTexture(0U, pD3DT.Get());
}

void Image::Impl::Write()
{
	//VRAMに書き込む時に外部から操作されないようにロックする
	void* pVoid;
	pD3DVB->Lock(
		0U,				//ロックするメモリのオフセット
		sizeof(v2D),	//ロックするメモリのサイズ
		&pVoid,			//ロックする（書き込む）メモリ（CreateVertexBufferで確保されたVRAMメモリ）
		0U				//ロック中のメモリ動作。特別な動作はない
		);

	//VRAMに書き込みなう
	memcpy(pVoid, v2D, sizeof(v2D));

	//ロックおわり
	pD3DVB->Unlock();
}

void Image::Impl::DrawImpl()
{
	pD3DDevice->SetStreamSource(
		0U,						//ストリーム番号
		pD3DVB.Get(),			//頂点バッファ
		0U,						//頂点バッファのオフセット
		sizeof(Impl::Vertex2D)	//一つの頂点のサイズ
	);

	pD3DDevice->SetFVF(Impl::D3DFVF_2DVERTEX);//頂点のフォーマット

	pD3DDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,	//頂点の結び方
		0U,						//開始する頂点
		2U						//必要な三角形の数。四角形を描画するので2つ
	);
}

Image::Image() : m_pImpl(new Impl)
{
	if(!m_pImpl->pD3DDevice && Graphic::Get()->Device().Get())
	{
		m_pImpl->pD3DDevice = Graphic::Get()->Device();
	}
}

Image::Image(const char* fileName) : m_pImpl(new Impl)
{
	if(!m_pImpl->pD3DDevice && Graphic::Get()->Device().Get())
	{
		m_pImpl->pD3DDevice = Graphic::Get()->Device();
	}

	Open(fileName);
}

Image::Image(const Image& rhs) : m_pImpl(new Impl(*rhs.m_pImpl))
{
}

Image& Image::operator=(const Image& rhs)
{
	Image temp(rhs);
	Swap(temp);
	return *this;
}

Image::~Image()
{
	Close();
}

void Image::Swap(Image& rhs)
{
	using std::swap;
	swap(m_pImpl, rhs.m_pImpl);
}

bool Image::Open(const char* fileName)
{
	//既に読み込んであるなら終わる
	if(m_pImpl->isDivide != indeterminate)
	{
		return false;
	}

	//テクスチャの座標。テクスチャの横100％、縦100％を使うという設定になっている
	m_pImpl->v2D[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	m_pImpl->v2D[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	m_pImpl->v2D[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	m_pImpl->v2D[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファの色の設定
	for(int i=0; i<4; ++i)
	{
		m_pImpl->v2D[i].dif = D3DXCOLOR(1.0f, 1.0f, 1.0f, (m_pImpl->alpha) / 255.0f);//頂点のディヒューズ色は全て白
		m_pImpl->v2D[i].rhw = 1.0f;			//Z座標は1.0f固定（最前面）
	}

	//ブレンドモードの設定
	m_pImpl->SetBlendMode(m_pImpl->blendMode);

	//画像の情報を取得
	D3DXGetImageInfoFromFile(_T(fileName), &(m_pImpl->D3DII));

	//頂点バッファの作成
	if(FAILED(m_pImpl->pD3DDevice->CreateVertexBuffer
		(
		4 * sizeof(Impl::Vertex2D),	//使用するメモリ（1頂点のサイズ*4つ）
		D3DUSAGE_WRITEONLY,			//頂点バッファの使い方。書き込み専用
		Impl::D3DFVF_2DVERTEX,		//頂点バッファのフォーマット
		D3DPOOL_MANAGED,			//使用メモリの管理方法
		m_pImpl->pD3DVB.ToCreate(),	//頂点バッファを管理するCOMのポインタのポインタ
		NULL						//未使用。必ずNULLを指定する
		)))
	{
		throw std::runtime_error("頂点バッファの作成に失敗しました");
	}

	//テクスチャステージの設定
	m_pImpl->SetTextureAlphaStageState();
	//テクスチャCOMの作成兼画像読み込み
	if(m_pImpl->alphaMode != ALPHAMODE_TRANS)
	{
		D3DXCreateTextureFromFile(m_pImpl->pD3DDevice.Get(), fileName, m_pImpl->pD3DT.ToCreate());
	}
	else
	{
		D3DXCreateTextureFromFileEx(
			m_pImpl->pD3DDevice.Get(),	//デバイス
			fileName,					//ファイル名
			m_pImpl->D3DII.Width,		//画像の幅
			m_pImpl->D3DII.Height,		//画像の高さ
			0U,							//ミニマップレベル
			0U,							//テクスチャの使い方
			D3DFMT_UNKNOWN,				//画像のフォーマット
			D3DPOOL_MANAGED,			//使用メモリの管理方法
			D3DX_DEFAULT,				//拡大した時の補間管方法を指定
			D3DX_DEFAULT,				//縮小した時の補間方法を指定
			m_pImpl->transColor,		//透過色を設定！！！
			&(m_pImpl->D3DII),			//画像情報を記録する
			NULL,						//画像が256色の時のみ使われる
			m_pImpl->pD3DT.ToCreate()	//テクスチャCOMのダブルポインタ
			);
	}
	//分割読み込みではない
	m_pImpl->isDivide = false;

	return true;
}

bool Image::Open(const char* fileName, int xNum, int yNum)
{
	//分割数が0以下だったり既に読み込み済みなら終わる
	if(xNum <= 0 || yNum <= 0 || m_pImpl->isDivide != indeterminate)
	{
		return false;
	}

	//分割数を覚えておく
	m_pImpl->xNum = xNum;
	m_pImpl->yNum = yNum;

	//頂点バッファの色の設定
	for(int i=0; i<4; ++i)
	{
		m_pImpl->v2D[i].dif = D3DXCOLOR(1.0f, 1.0f, 1.0f, (m_pImpl->alpha) / 255.0f);//頂点のディヒューズ色は全て白
		m_pImpl->v2D[i].rhw = 1.0f;			//Z座標は1.0f固定（最前面）
	}

	//ブレンドモードの設定
	m_pImpl->SetBlendMode(m_pImpl->blendMode);

	//画像の情報を取得
	D3DXGetImageInfoFromFile(_T(fileName), &(m_pImpl->D3DII));

	//1枚の画像サイズを求める
	m_pImpl->divideWidth = m_pImpl->D3DII.Width / static_cast<float>(xNum);
	m_pImpl->divideHeight = m_pImpl->D3DII.Height / static_cast<float>(yNum);

	//頂点バッファの作成
	if(FAILED(m_pImpl->pD3DDevice->CreateVertexBuffer
		(
		4 * sizeof(Impl::Vertex2D),	//使用するメモリ（1頂点のサイズ*4つ）
		D3DUSAGE_WRITEONLY,			//頂点バッファの使い方。書き込み専用
		Impl::D3DFVF_2DVERTEX,		//頂点バッファのフォーマット
		D3DPOOL_MANAGED,			//使用メモリの管理方法
		m_pImpl->pD3DVB.ToCreate(),	//頂点バッファを管理するCOMのポインタのポインタ
		NULL						//未使用。必ずNULLを指定する
		)))
	{
		throw std::runtime_error("頂点バッファの作成に失敗しました");
	}

	//テクスチャステージの設定
	m_pImpl->SetTextureAlphaStageState();
	//テクスチャCOMの作成兼画像読み込み
	if(m_pImpl->alphaMode != ALPHAMODE_TRANS)
	{
		D3DXCreateTextureFromFile(m_pImpl->pD3DDevice.Get(), fileName, m_pImpl->pD3DT.ToCreate());
	}
	else
	{
		D3DXCreateTextureFromFileEx(
			m_pImpl->pD3DDevice.Get(),		//デバイス
			fileName,					//ファイル名
			m_pImpl->D3DII.Width,		//画像の幅
			m_pImpl->D3DII.Height,		//画像の高さ
			0U,							//ミニマップレベル
			0U,							//テクスチャの使い方
			D3DFMT_UNKNOWN,				//画像のフォーマット
			D3DPOOL_MANAGED,			//使用メモリの管理方法
			D3DX_DEFAULT,				//拡大した時の補間管方法を指定
			D3DX_DEFAULT,				//縮小した時の補間方法を指定
			m_pImpl->transColor,		//透過色を設定！！！
			&(m_pImpl->D3DII),			//画像情報を記録する
			NULL,						//画像が256色の時のみ使われる
			m_pImpl->pD3DT.ToCreate()	//テクスチャCOMのダブルポインタ
			);
	}
	//分割読み込みである
	m_pImpl->isDivide = true;

	m_pImpl->divideWidthRate = 1.0f / m_pImpl->xNum;
	m_pImpl->divideHeightRate = 1.0f / m_pImpl->yNum;

	return true;
}

bool Image::Close()
{
	return true;
}

bool Image::Draw(const Vector& position, bool isTrans)
{
	//初期化、エラーチェック
	m_pImpl->DrawInit();

	//表示する頂点座標の設定
	float w = static_cast<float>(m_pImpl->D3DII.Width);
	float h = static_cast<float>(m_pImpl->D3DII.Height);

	//名前を短く
	float x = position.x;
	float y = position.y;

	//反転しない
	if(!isTrans)
	{
		m_pImpl->v2D[0].pos = D3DXVECTOR3(x    , y    , 0.0f);
		m_pImpl->v2D[1].pos = D3DXVECTOR3(x + w, y    , 0.0f);
		m_pImpl->v2D[2].pos = D3DXVECTOR3(x    , y + h, 0.0f);
		m_pImpl->v2D[3].pos = D3DXVECTOR3(x + w, y + h, 0.0f);
	}
	else
	{
		m_pImpl->v2D[0].pos = D3DXVECTOR3(x + w, y    , 0.0f);
		m_pImpl->v2D[1].pos = D3DXVECTOR3(x    , y    , 0.0f);
		m_pImpl->v2D[2].pos = D3DXVECTOR3(x + w, y + h, 0.0f);
		m_pImpl->v2D[3].pos = D3DXVECTOR3(x    , y + h, 0.0f);
		//カリングを無効にする
		m_pImpl->pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}

	//VRAMへ書き込み
	m_pImpl->Write();

	//描画
	m_pImpl->DrawImpl();

	//カリングを元に戻す
	if(isTrans)
	{
		m_pImpl->pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}

	return true;
}

bool Image::Draw(const Vector& position, float angle, float rate, bool isTrans)
{
	//初期化、エラーチェック
	m_pImpl->DrawInit();

	//表示する頂点座標の設定
	float w = static_cast<float>(m_pImpl->D3DII.Width) * rate;
	float h = static_cast<float>(m_pImpl->D3DII.Height) * rate;

	//回転した後の座標の設定
	float X[3];	//回転後のx座標
	float Y[3];	//回転後のy座標
	float c = cos(angle);
	float s = sin(-angle);

	X[0] = w * c;
	Y[0] = w * s;
	X[1] = -h * s;
	Y[1] = h * c;
	X[2] = w * c - h * s;
	Y[2] = w * s + h * c;

	//名前を短く
	float x = position.x;
	float y = position.y;

	//反転しない
	if(!isTrans)
	{
		m_pImpl->v2D[0].pos = D3DXVECTOR3(x , y , 0.0f);
		m_pImpl->v2D[1].pos = D3DXVECTOR3(X[0] + x , Y[0] + y , 0.0f);
		m_pImpl->v2D[2].pos = D3DXVECTOR3(X[1] + x , Y[1] + y , 0.0f);
		m_pImpl->v2D[3].pos = D3DXVECTOR3(X[2] + x , Y[2] + y , 0.0f);
	}
	else
	{
		m_pImpl->v2D[0].pos = D3DXVECTOR3(X[0] + x , Y[0] + y , 0.0f);
		m_pImpl->v2D[1].pos = D3DXVECTOR3(x , y , 0.0f);
		m_pImpl->v2D[2].pos = D3DXVECTOR3(X[2] + x , Y[2] + y , 0.0f);
		m_pImpl->v2D[3].pos = D3DXVECTOR3(X[1] + x , Y[1] + y , 0.0f);

		//カリングを無効にする
		m_pImpl->pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}

	//VRAMへ書き込み
	m_pImpl->Write();

	//描画
	m_pImpl->DrawImpl();

	//カリングを元に戻す
	if(isTrans)
	{
		m_pImpl->pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}

	return true;
}

bool Image::Draw(const Vector& position, float angle, float rate, const Vector& center, bool isTrans)
{
	//初期化、エラーチェック
	m_pImpl->DrawInit();

	//表示する頂点座標の設定
	float w = static_cast<float>(m_pImpl->D3DII.Width) * rate;
	float h = static_cast<float>(m_pImpl->D3DII.Height) * rate;

	
	//回転した後の座標の設定

	float X[4];	//回転後x座標
	float Y[4];	//回転後y座標
	float c = cos(angle);//cosを予め計算
	float s = sin(-angle);//sinを予め計算

	//名前を短く
	float cx = center.x;
	float cy = center.y;

	X[0] = (-cx) * c - (-cy) * s + cx;
	Y[0] = (-cx) * s + (-cy) * c + cy;

	X[1] = (w - cx) * c - (-cy) * s + cx;
	Y[1] = (w - cx) * s + (-cy) * c + cy;

	X[2] = (-cx) * c - (h - cy) * s + cx;
	Y[2] = (-cx) * s + (h - cy) * c + cy;
		
	X[3] = (w - cx) * c - (h - cy) * s + cx;
	Y[3] = (w - cx) * s + (h - cy) * c + cy;

	//名前を短く
	float x = position.x;
	float y = position.y;

	//反転しない
	if(!isTrans)
	{
		m_pImpl->v2D[0].pos = D3DXVECTOR3(X[0] + x, Y[0] + y, 0.0f);
		m_pImpl->v2D[1].pos = D3DXVECTOR3(X[1] + x, Y[1] + y, 0.0f);
		m_pImpl->v2D[2].pos = D3DXVECTOR3(X[2] + x, Y[2] + y, 0.0f);
		m_pImpl->v2D[3].pos = D3DXVECTOR3(X[3] + x, Y[3] + y, 0.0f);
	}
	else
	{
		m_pImpl->v2D[0].pos = D3DXVECTOR3(X[1] + x, Y[1] + y, 0.0f);
		m_pImpl->v2D[1].pos = D3DXVECTOR3(X[0] + x, Y[0] + y, 0.0f);
		m_pImpl->v2D[2].pos = D3DXVECTOR3(X[3] + x, Y[3] + y, 0.0f);
		m_pImpl->v2D[3].pos = D3DXVECTOR3(X[2] + x, Y[2] + y, 0.0f);

		//カリングを無効にする
		m_pImpl->pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}

	//VRAMへ書き込み
	m_pImpl->Write();

	//描画
	m_pImpl->DrawImpl();

	if(isTrans)
	{
		//カリングを元に戻す
		m_pImpl->pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}

	return true;
}

bool Image::Draw(int num, const Vector& position, bool isTrans)
{
	//初期化、エラーチェック
	m_pImpl->DrawInit(num);

	//表示する頂点座標の設定
	float w = static_cast<float>(m_pImpl->divideWidth);
	float h = static_cast<float>(m_pImpl->divideHeight);

	//使うテクスチャ座標を計算

	//名前を短く
	float tw = m_pImpl->divideWidthRate;
	float th = m_pImpl->divideHeightRate;
	
	//表示するテクスチャの左上座標を求める
	float tx = (num % static_cast<int>(m_pImpl->xNum)) * tw;
	float ty = (num / static_cast<int>(m_pImpl->xNum)) * th;

	m_pImpl->v2D[0].tex = D3DXVECTOR2(tx     , ty     );
	m_pImpl->v2D[1].tex = D3DXVECTOR2(tx + tw, ty     );
	m_pImpl->v2D[2].tex = D3DXVECTOR2(tx     , ty + th);
	m_pImpl->v2D[3].tex = D3DXVECTOR2(tx + tw, ty + th);

	//名前を短く
	float x = position.x;
	float y = position.y;

	//反転しない
	if(!isTrans)
	{
		m_pImpl->v2D[0].pos = D3DXVECTOR3(x    , y    , 0.0f);
		m_pImpl->v2D[1].pos = D3DXVECTOR3(x + w, y    , 0.0f);
		m_pImpl->v2D[2].pos = D3DXVECTOR3(x    , y + h, 0.0f);
		m_pImpl->v2D[3].pos = D3DXVECTOR3(x + w, y + h, 0.0f);
	}
	else
	{
		m_pImpl->v2D[0].pos = D3DXVECTOR3(x + w, y    , 0.0f);
		m_pImpl->v2D[1].pos = D3DXVECTOR3(x    , y    , 0.0f);
		m_pImpl->v2D[2].pos = D3DXVECTOR3(x + w, y + h, 0.0f);
		m_pImpl->v2D[3].pos = D3DXVECTOR3(x    , y + h, 0.0f);
		//カリングを無効にする
		m_pImpl->pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}

	//VRAMへ書き込み
	m_pImpl->Write();

	//描画
	m_pImpl->DrawImpl();

	//カリングを元に戻す
	if(isTrans)
	{
		m_pImpl->pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}

	return true;
}

bool Image::Draw(int num, const Vector& position, float angle, float rate, bool isTrans)
{
	//初期化、エラーチェック
	m_pImpl->DrawInit(num);

	//表示する頂点座標の設定
	float w = static_cast<float>(m_pImpl->divideWidth) * rate;
	float h = static_cast<float>(m_pImpl->divideHeight) * rate;

	//使うテクスチャ座標を計算

	//名前を短く
	float tw = m_pImpl->divideWidthRate;
	float th = m_pImpl->divideHeightRate;
	
	//表示するテクスチャの左上座標を求める
	float tx = (num % static_cast<int>(m_pImpl->xNum)) * tw;
	float ty = (num / static_cast<int>(m_pImpl->xNum)) * th;

	m_pImpl->v2D[0].tex = D3DXVECTOR2(tx     , ty     );
	m_pImpl->v2D[1].tex = D3DXVECTOR2(tx + tw, ty     );
	m_pImpl->v2D[2].tex = D3DXVECTOR2(tx     , ty + th);
	m_pImpl->v2D[3].tex = D3DXVECTOR2(tx + tw, ty + th);
	
	//回転した後の座標の設定
	float X[3];	//回転後のx座標
	float Y[3];	//回転後のy座標
	float c = cos(angle);
	float s = sin(-angle);

	X[0] = w * c;
	Y[0] = w * s;
	X[1] = -h * s;
	Y[1] = h * c;
	X[2] = w * c - h * s;
	Y[2] = w * s + h * c;

	//名前を短く
	float x = position.x;
	float y = position.y;

	//反転しない
	if(!isTrans)
	{
		m_pImpl->v2D[0].pos = D3DXVECTOR3(x        , y        , 0.0f);
		m_pImpl->v2D[1].pos = D3DXVECTOR3(X[0] + x , Y[0] + y , 0.0f);
		m_pImpl->v2D[2].pos = D3DXVECTOR3(X[1] + x , Y[1] + y , 0.0f);
		m_pImpl->v2D[3].pos = D3DXVECTOR3(X[2] + x , Y[2] + y , 0.0f);
	}
	else
	{
		m_pImpl->v2D[0].pos = D3DXVECTOR3(X[0] + x , Y[0] + y , 0.0f);
		m_pImpl->v2D[1].pos = D3DXVECTOR3(x        , y        , 0.0f);
		m_pImpl->v2D[2].pos = D3DXVECTOR3(X[2] + x , Y[2] + y , 0.0f);
		m_pImpl->v2D[3].pos = D3DXVECTOR3(X[1] + x , Y[1] + y , 0.0f);

		//カリングを無効にする
		m_pImpl->pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}

	//VRAMへ書き込み
	m_pImpl->Write();

	//描画
	m_pImpl->DrawImpl();

	return true;
}

bool Image::Draw(int num, const Vector& position, float angle, float rate, const Vector& center, bool isTrans)
{
	//初期化、エラーチェック
	m_pImpl->DrawInit(num);

	//表示する頂点座標の設定
	float w = static_cast<float>(m_pImpl->divideWidth) * rate;
	float h = static_cast<float>(m_pImpl->divideHeight) * rate;

	//使うテクスチャ座標を計算

	//名前を短く
	float tw = m_pImpl->divideWidthRate;
	float th = m_pImpl->divideHeightRate;
	
	//表示するテクスチャの左上座標を求める
	float tx = (num % static_cast<int>(m_pImpl->xNum)) * tw;
	float ty = (num / static_cast<int>(m_pImpl->xNum)) * th;

	m_pImpl->v2D[0].tex = D3DXVECTOR2(tx     , ty     );
	m_pImpl->v2D[1].tex = D3DXVECTOR2(tx + tw, ty     );
	m_pImpl->v2D[2].tex = D3DXVECTOR2(tx     , ty + th);
	m_pImpl->v2D[3].tex = D3DXVECTOR2(tx + tw, ty + th);
	
	//回転した後の座標の設定

	float X[4];	//回転後x座標
	float Y[4];	//回転後y座標
	float c = cos(angle);//cosを予め計算
	float s = sin(-angle);//sinを予め計算

	//名前を短く
	float cx = center.x;
	float cy = center.y;

	X[0] = (-cx) * c - (-cy) * s + cx;
	Y[0] = (-cx) * s + (-cy) * c + cy;

	X[1] = (w - cx) * c - (-cy) * s + cx;
	Y[1] = (w - cx) * s + (-cy) * c + cy;

	X[2] = (-cx) * c - (h - cy) * s + cx;
	Y[2] = (-cx) * s + (h - cy) * c + cy;
		
	X[3] = (w - cx) * c - (h - cy) * s + cx;
	Y[3] = (w - cx) * s + (h - cy) * c + cy;

	//名前を短く
	float x = position.x;
	float y = position.y;

	//反転しない
	if(!isTrans)
	{
		m_pImpl->v2D[0].pos = D3DXVECTOR3(X[0] + x, Y[0] + y, 0.0f);
		m_pImpl->v2D[1].pos = D3DXVECTOR3(X[1] + x, Y[1] + y, 0.0f);
		m_pImpl->v2D[2].pos = D3DXVECTOR3(X[2] + x, Y[2] + y, 0.0f);
		m_pImpl->v2D[3].pos = D3DXVECTOR3(X[3] + x, Y[3] + y, 0.0f);
	}
	else
	{
		m_pImpl->v2D[0].pos = D3DXVECTOR3(X[1] + x, Y[1] + y, 0.0f);
		m_pImpl->v2D[1].pos = D3DXVECTOR3(X[0] + x, Y[0] + y, 0.0f);
		m_pImpl->v2D[2].pos = D3DXVECTOR3(X[3] + x, Y[3] + y, 0.0f);
		m_pImpl->v2D[3].pos = D3DXVECTOR3(X[2] + x, Y[2] + y, 0.0f);

		//カリングを無効にする
		m_pImpl->pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}

	//VRAMへ書き込み
	m_pImpl->Write();

	//描画
	m_pImpl->DrawImpl();

	if(isTrans)
	{
		//カリングを元に戻す
		m_pImpl->pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}

	return true;
}

void Image::BlendMode(BLENDMODE blendMode)
{
	m_pImpl->blendMode = blendMode;
}

void Image::Alpha(unsigned char alpha)
{
	if(m_pImpl->alpha != alpha)
	{
		m_pImpl->alpha = alpha;
	}
}

void Image::AlphaMode(ALPHAMODE alphaMode)
{
	m_pImpl->alphaMode = alphaMode;
}

void Image::TransColor(unsigned int color)
{
	m_pImpl->transColor = color;
}

int Image::Width() const
{
	return m_pImpl->isDivide ? static_cast<int>(m_pImpl->divideWidth) : static_cast<int>(m_pImpl->D3DII.Width);
}

int Image::Height() const
{
	return m_pImpl->isDivide ? static_cast<int>(m_pImpl->divideHeight) : static_cast<int>(m_pImpl->D3DII.Height);
}