#include"font.h"

#include"transiter.h"


namespace Maid
{

  static const int s_SCALE = 4;
void Font::Create( const SIZE2DI& size, bool IsAntiAlias )
{
  const String name = GlobalPointer<GraphicsCore>::Get()->GetFontDevice()->GetDefaultFontName();

  Create( name, size, IsAntiAlias );
}

void Font::Create( const String& Name, const SIZE2DI& size, bool IsAntiAlias )
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
}

void Font::Destroy()
{
  m_pFont.reset();
}

const String& Font::GetName()   const
{
  return m_FontName;
}

const SIZE2DI& Font::GetSize()   const
{
  return m_FontSize;
}

bool Font::IsAntiAlias()const
{
  return m_IsAntiAlias;
}

void Font::Rasterize( unt32 code, const FONTRASTERIZEFILTER& Filter, Surface& Dst )const
{
  SPSURFACE pMem( new Surface );

  GlobalPointer<GraphicsCore>::Get()->GetFontDevice()->Rasterize( m_pFont, code, *pMem );

  for( size_t i=0; i<Filter.size(); ++i )
  {
    Filter[i]->Convert( pMem );
  }

  const SIZE2DI size(pMem->GetSize().w/s_SCALE,pMem->GetSize().h/s_SCALE);

  Dst.Create( size, pMem->GetPixelFormat() );
  Transiter::Bilinear( *pMem, Dst );
}


}
