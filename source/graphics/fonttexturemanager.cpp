﻿#include"fonttexturemanager.h"


#include"../auxiliary/debug/assert.h"
#include"../auxiliary/debug/warning.h"

#include"../auxiliary/string.h"


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

  //  フォントは縮小しなくてもいいと思う
  const SIZE2DI ImgSize = surf.GetSize();
  const SIZE2DI TexSize = pCore->CalcTextureSize( ImgSize );

  {
    tex.Size = TexSize;
    tex.MipLevels = 1;
    tex.ArraySize = 1;
    tex.Format = DstFormat;
    tex.Usage  = Graphics::RESOURCEUSAGE_DEFAULT;
    tex.BindFlags = Graphics::RESOURCEBINDFLAG_MATERIAL;

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

    pMaterial = pCore->GetDevice()->CreateMaterial( pTexture, NULL );
  }

  Setup( pTexture, pMaterial, ImgSize, ImgSize, surf.GetPixelFormat() );

}

void FontTexture::Destroy()
{
  IMaterial::Clear();
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
const FontTexture& FontTextureManager::GetTexture( const Font& f, unt32 FontCode )
{
	const CACHEKEY MapKey(f,FontCode);

	if( m_CacheTable.IsExist(MapKey) )
	{
		return m_CacheTable.GetData(MapKey);
	}


  SurfaceInstance surf; //  フォントとして使うデータ

  f.Rasterize( FontCode, surf );


	FontTexture tex;
	
	tex.Load( surf );

	m_CacheTable.Register( MapKey, tex );

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
void FontTextureManager::Initialize()
{
	m_CacheTable.SetCacheSize( 200 );
	m_CacheTable.SetGarbageState( CACHETABLE::GARBAGESTATE_MANUAL );
}

void FontTextureManager::SetFilter( const FONTRASTERIZEFILTER& Filter )
{
  m_Filter = Filter;
	m_CacheTable.ClearAll();
}

void FontTextureManager::Finalize()
{
	m_CacheTable.ClearAll();
}

FontTextureManager::CACHEKEY::CACHEKEY()
  :Code(0)
{
}

FontTextureManager::CACHEKEY::CACHEKEY( const CACHEKEY& rha )
  :font(rha.font),Code(rha.Code)
{
}

FontTextureManager::CACHEKEY::CACHEKEY( const Font& f, unt32 c )
  :font(f),Code(c)
{
}

bool FontTextureManager::CACHEKEY::operator < ( const CACHEKEY& rhs ) const
{
	if( Code < rhs.Code ) { return true; }
	else if( Code > rhs.Code ) { return false; }

	if( font.GetName() < rhs.font.GetName() ) { return true; }
	else if( font.GetName() > rhs.font.GetName() ) { return false; }

	if( font.GetSize().w  < rhs.font.GetSize().w  ) { return true; }
	else if( font.GetSize().w  > rhs.font.GetSize().w  ) { return false; }

	if( font.GetSize().h < rhs.font.GetSize().h ) { return true; }
	else if( font.GetSize().h > rhs.font.GetSize().h ) { return false; }

	if( font.GetFilterNo() < rhs.font.GetFilterNo() ) { return true; }
	else if( font.GetFilterNo() > rhs.font.GetFilterNo() ) { return false; }

	if( font.IsAntiAlias() != rhs.font.IsAntiAlias() ) { return true; }
	return false;
}

}