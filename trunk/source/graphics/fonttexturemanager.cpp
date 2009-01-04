#include"fonttexturemanager.h"


#include"../auxiliary/debug/assert.h"
#include"../auxiliary/debug/warning.h"

#include"../auxiliary/string.h"

#include"transiter.h"

namespace Maid
{


/*!
 	@class	FontTexture fonttexturecache.h
 	@brief	FontTextureCache 内で管理されるテクスチャ
 */

void FontTexture::Load( const Surface& surf )
{
  GraphicsCore* pCore = GlobalPointer<GraphicsCore>::Get();
	Destroy();

  Graphics::SPTEXTURE2D pTexture;

  Graphics::CREATERETEXTURE2DPARAM tex;
  Graphics::CREATEMATERIALPARAM mat;
  const PIXELFORMAT DstFormat = pCore->FindFormatTexture2D( surf.GetPixelFormat(), false );

  {

    tex.Size = pCore->CalcTextureSize(surf.GetSize());
    tex.MipLevels = 0;
    tex.ArraySize = 0;
    tex.Format = DstFormat;
    tex.Usage = Graphics::RESOURCEUSAGE_IMMUTABLE;
    tex.BindFlags = Graphics::RESOURCEBINDFLAG_MATERIAL;
    tex.MiscFlags = Graphics::RESOURCEMISCFLAG_GENERATE_MIPS;

    Graphics::SUBRESOURCE Data;
    Data.pData = const_cast<Surface&>(surf).GetPlanePTR();
    Data.Pitch = const_cast<Surface&>(surf).GetPitch();
    pTexture = pCore->GetDevice()->CreateTexture2D( tex, &Data );
  }

  if( pTexture.get()==NULL )
  {
      MAID_WARNING( MAIDTEXT("テクスチャの作成に失敗 ") );
      return ; 
  }

  Graphics::SPMATERIAL pMaterial;

  {
    mat.Format = DstFormat;
    mat.Dimension =  Graphics::CREATEMATERIALPARAM::DIMENSION_TEXTURE2D;

    pMaterial = pCore->GetDevice()->CreateMaterial( pTexture, mat );
  }

  IMaterial::Set( pMaterial );

  m_FontSize = surf.GetSize();
  m_CreateSize = m_FontSize;
  m_TextureSize = tex.Size;
}

void FontTexture::Destroy()
{
  IMaterial::Clear();
}

const SIZE2DI& FontTexture::GetFontSize()const
{
  MAID_ASSERT( IMaterial::Get().get()==NULL, MAIDTEXT("まだ作成されていません") );
	return m_FontSize;
}

const SIZE2DI& FontTexture::GetCreateSize() const
{
  MAID_ASSERT( IMaterial::Get().get()==NULL, MAIDTEXT("まだ作成されていません") );
	return m_CreateSize;
}


const SIZE2DI& FontTexture::GetTextureSize()const
{
	MAID_ASSERT( IMaterial::Get().get()==NULL, MAIDTEXT("まだ作成されていません") );
	return m_TextureSize;
}


/*!
 	@class	FontTextureManager fonttexturecache.h
 	@brief	FontTexture の管理クラス
\n			こいつがうまくやりくりする
 */

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! コンストラクタ
/*!
 */
FontTextureManager::FontTextureManager()
{
	m_CacheTable.SetCacheSize( 200 );
	m_CacheTable.SetGarbageState( CACHETABLE::GARBAGESTATE_MANUAL );
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! デストラクタ
/*!
 */
FontTextureManager::~FontTextureManager()
{

}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! フォントテクスチャの取得
/*!
 	この関数を呼び出すだけで自動的に、キャッシュの破棄、作成、更新
\n	を行ってくれます
 
 	@param	pFont		[i ]	取得したいフォント
 	@param	FontCode	[i ]	取得したいフォントコード
 */
const FontTexture& FontTextureManager::GetTexture( const Graphics::SPFONT& pFont, unt32 FontCode )
{
	const CACHEKEY MapKey(pFont,FontCode);

	if( m_CacheTable.IsExist(MapKey) )
	{
		return m_CacheTable.GetData(MapKey);
	}


  Surface surf; //  フォントとして使うデータ

  {
    //  アンチエイリアスをかけるため、縦横２倍のサイズで作って
    //  バイリニアフィルタで縮小する
    Graphics::SPFONT pTmp = m_pFontDevice->CreateFont( pFont->GetName(), SIZE2DI(pFont->GetSize().w*4, pFont->GetSize().h*4) );

    SPSURFACE pMem( new Surface );

    m_pFontDevice->Rasterize( pTmp, FontCode, *pMem );

    for( size_t i=0; i<m_Filter.size(); ++i )
    {
      m_Filter[i]->Convert( pMem );
    }

    surf.Create( SIZE2DI(pMem->GetSize().w/4, pMem->GetSize().h/4) , pMem->GetPixelFormat() );
    Transiter::Bilinear( *pMem, surf );
  }


	FontTexture tex;
	
	tex.Load( surf );

	m_CacheTable.Regist( MapKey, tex );

	return m_CacheTable.GetData(MapKey);
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! キャッシュをすべてクリアする
/*!
 */
void FontTextureManager::ClearCache()
{
	m_CacheTable.ClearAll();
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! キャッシュとして保持する数を変更する
/*!
 	@param	Size			[i ]	最低限キャッシュしておく数
 */
void FontTextureManager::SetCacheSize( int Size )
{
	m_CacheTable.SetCacheSize(Size);
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! キャッシュの掃除
/*!
 	@_CacheMax 個まで減少させます
 */
void FontTextureManager::Garbage()
{
	m_CacheTable.Garbage();
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 初期化
/*!
 */
void FontTextureManager::Initialize( const Graphics::SPFONTDEVICE& pFont )
{
	m_pFontDevice = pFont;
}

void FontTextureManager::SetFilter( const FONTFILTER& Filter )
{
  m_Filter = Filter;
	m_CacheTable.ClearAll();
}

void FontTextureManager::Finalize()
{
	m_CacheTable.ClearAll();
	m_pFontDevice.reset();
}

FontTextureManager::CACHEKEY::CACHEKEY()
  :pFont(Graphics::SPFONT()),Code(0)
{
}

FontTextureManager::CACHEKEY::CACHEKEY( const CACHEKEY& rha )
  :pFont(rha.pFont),Code(rha.Code)
{
}

FontTextureManager::CACHEKEY::CACHEKEY( const Graphics::SPFONT& p, unt32 c )
  :pFont(p),Code(c)
{
}

bool FontTextureManager::CACHEKEY::operator < ( const CACHEKEY& rhs ) const
{
	if( Code < rhs.Code ) { return true; }
	ef( Code > rhs.Code ) { return false; }

	if( pFont->GetName() < rhs.pFont->GetName() ) { return true; }
	ef( pFont->GetName() > rhs.pFont->GetName() ) { return false; }

	if( pFont->GetSize().w  < rhs.pFont->GetSize().w  ) { return true; }
	ef( pFont->GetSize().w  > rhs.pFont->GetSize().w  ) { return false; }

	if( pFont->GetSize().h < rhs.pFont->GetSize().h ) { return true; }
	ef( pFont->GetSize().h > rhs.pFont->GetSize().h ) { return false; }

	return false;
}

}