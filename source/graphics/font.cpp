#include"font.h"

#include"transiter.h"

#pragma COMPILERMSG("TODO:任意段階のアンチエイリアスがかけれるようにする")	

namespace Maid
{

Font::FILTERMAP  Font::s_FilterMap;

static const int s_SCALE = 4;


//! フォントを作成する
/*!
    フォント名はデフォルトです。
    
    @param size         [i ]  フォントの大きさ
    @param IsAntiAlias  [i ]  アンチエイリアスをかけるか
 */
void Font::Create( const SIZE2DI& size, bool IsAntiAlias )
{
  Create( size, IsAntiAlias, FILTER_NONE );
}

//! フォントを作成する
/*!
    フォント名はデフォルトです。
    
    @param size         [i ]  フォントの大きさ
    @param IsAntiAlias  [i ]  アンチエイリアスをかけるか
    @param FilterNo     [i ]  おこなうフィルタ処理( SetFilter() で設定したやつ )
 */
void Font::Create( const SIZE2DI& size, bool IsAntiAlias, unt FilterNo )
{
  const String name = GlobalPointer<GraphicsCore>::Get()->GetFontDevice()->GetDefaultFontName();

  Create( name, size, IsAntiAlias, FilterNo );
}


//! フォントを作成する
/*!
    @param Name         [i ]  作成するフォント名
    @param size         [i ]  フォントの大きさ
    @param IsAntiAlias  [i ]  アンチエイリアスをかけるか
    @param FilterNo     [i ]  おこなうフィルタ処理( SetFilter() で設定したやつ )
 */
void Font::Create( const String& Name, const SIZE2DI& size, bool IsAntiAlias, unt FilterNo )
{
  SIZE2DI createsize(size);

  //  アンチエイリアスをかける場合、４倍に広げてから、縮小でかけるので
  //  内部的にはs_SCALE倍の大きさで持っておく
  if( IsAntiAlias )
  {
    createsize.w *= s_SCALE;
    createsize.h *= s_SCALE;
  }

  m_pFont = GlobalPointer<GraphicsCore>::Get()->GetFontDevice()->CreateFont( Name, createsize );

  m_FontName = Name;
  m_FontSize = size;
  m_IsAntiAlias = IsAntiAlias;
  m_FilterNo = FilterNo;
}

//! フォントの破棄
/*!
 */
void Font::Destroy()
{
  m_pFont.reset();
}


//! 作成しているフォント名の取得
/*!
 */
const String& Font::GetName()   const
{
  return m_FontName;
}

//! 作成しているフォントサイズの取得
/*!
 */
const SIZE2DI& Font::GetSize()   const
{
  return m_FontSize;
}

//! アンチエイリアスがかかっているか？
/*!
 */
bool Font::IsAntiAlias()const
{
  return m_IsAntiAlias;
}


//! かけるフィルタ番号
/*!
 */
unt Font::GetFilterNo() const
{
  return m_FilterNo;
}

//! ラスタライズする
/*!
    @param code  [i ]  ラスタライズする文字コード（MSTRING)
    @param Dst   [ o]  ラスタライズ先
 */
void Font::Rasterize( unt32 code, Surface& Dst )const
{
  SPSURFACE pMem( new Surface );

  GlobalPointer<GraphicsCore>::Get()->GetFontDevice()->Rasterize( m_pFont, code, *pMem );

  if( m_FilterNo!=FILTER_NONE )
  {
    FILTERMAP::const_iterator ite = s_FilterMap.find(m_FilterNo);
    if( ite!=s_FilterMap.end() )
    {
      const FONTRASTERIZEFILTER& Filter = ite->second;
      for( size_t i=0; i<Filter.size(); ++i )
      {
        Filter[i]->Convert( pMem );
      }
    }
  }

  const SIZE2DI size(pMem->GetSize().w/s_SCALE,pMem->GetSize().h/s_SCALE);

  Dst.Create( size, pMem->GetPixelFormat() );
  Transiter::Average( *pMem, Dst );
}

//! ラスタライズフィルタの設定
/*!
    @param No     [i ]  設定番号
    @param Filter [i ]  フィルタ
 */
void Font::SetFilter( unt No, const FONTRASTERIZEFILTER& Filter )
{
  s_FilterMap[No] = Filter;
}


}
