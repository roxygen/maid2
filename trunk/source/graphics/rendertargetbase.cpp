#include"rendertargetbase.h"
#include"../auxiliary/debug/warning.h"
#include"../auxiliary/debug/assert.h"
#include"../auxiliary/mathematics/point.h"

namespace Maid
{

  /*!
      @class  RenderTargetBase rendertargetbase.h
      @brief  描画対象となる平面をあらわす基本クラス
    \n        実際の大きさと異なるように振舞ってほしい場合、このクラスを継承して使うとベンリ
  */
RenderTargetBase::RenderTargetBase()
  :m_Size(0,0)
  ,m_TextureSize(0,0)
  ,m_Area(DRAWAREA_LEFTTOP)
{

}


//! 現在の設定で描画範囲を計算する
/*!
    描画範囲は中心になります。
 */
RECT2DI RenderTargetBase::CalcDrawArea() const
{
  const SIZE2DI& s = GetSize();
  const SIZE2DI& t = GetTextureSize();

  POINT2DI pos;
  SIZE2DI size;

  if( s.w <= t.w && s.h <= t.h )
  { //  ユーザーが指定したサイズよりも実際に作成したサイズの方が大きい場合
    pos = POINT2DI( t.w-s.w, t.h-s.h );
    size= s;
  }else
  { //  ユーザーが指定したサイズよりも実際に作成したサイズの方が小さい場合
    const int test_h = t.w * s.h / s.w; //  横幅を固定して縦を求めた

    if( test_h <= t.h )
    {
      pos  = POINT2DI( 0, t.h-test_h );
      size = SIZE2DI(t.w,test_h);
    }else
    {
      const int w = t.h * s.w / s.h;
      pos  = POINT2DI( t.w-w, 0 );
      size = SIZE2DI(w,t.h);
    }
  }

  RECT2DI ret;

  ret.w = size.w;
  ret.h = size.h;

  switch( m_Area )
  {
  case DRAWAREA_CENTER:     { ret.x = pos.x/2;  ret.y = pos.y/2; }break;
  case DRAWAREA_LEFTTOP:    { ret.x =       0;  ret.y =       0; }break;
  case DRAWAREA_RIGHTTOP:   { ret.x =   pos.x;  ret.y =       0; }break;
  case DRAWAREA_LEFTBOTTOM: { ret.x =       0;  ret.y =  pos.y; }break;
  case DRAWAREA_RIGHTBOTTOM:{ ret.x =   pos.x;  ret.y =  pos.y; }break;
  }

/*
  RECT2DI ret;

  if( s.w <= t.w && s.h <= t.h )
  { //  ユーザーが指定したサイズよりも実際に作成したサイズの方が大きい場合
    const POINT2DI pos( (t.w-s.w) / 2, (t.h-s.h) / 2 );
    ret = RECT2DI(pos, s);
  }else
  { //  ユーザーが指定したサイズよりも実際に作成したサイズの方が小さい場合
    const int test_h = t.w * s.h / s.w; //  横幅を固定して縦を求めた

    if( test_h <= t.h )
    {
      const POINT2DI pos( 0, (t.h-test_h) / 2 );
      ret = RECT2DI(pos, SIZE2DI(t.w,test_h));
    }else
    {
      const int w = t.h * s.w / s.h;
      const POINT2DI pos( (t.w-w) / 2, 0 );
      ret = RECT2DI(pos, SIZE2DI(w,t.h));
    }
  }
*/


  return ret;
}


//! 作っていたものを開放する
/*!
 */
void RenderTargetBase::Clear()
{
  IRenderTarget::Clear();

  m_Size = SIZE2DI(0,0);
  m_TextureSize = SIZE2DI(0,0);
}


//! すでに作成されているか？
/*!
    @return 作成されているなら true
 */
bool RenderTargetBase::IsSetupped() const
{
  return IRenderTarget::Get().get()!=NULL;
}

//! 各種設定しておく
/*!
    @param  pTarget [i ]  要求があったときに差し出すターゲット
    @param  create  [i ]  扱って欲しい平面の大きさ
 */
void RenderTargetBase::Setup( const Graphics::SPRENDERTARGET& pTarget, const SIZE2DI& create, DRAWAREA area )
{
  MAID_ASSERT( pTarget->GetResource()->GetType()!=Graphics::IResource::TYPE_TEXTURE2D,
      "設定するリソースがテクスチャではありません" );
  IRenderTarget::Set(pTarget);

  const Graphics::CREATERETEXTURE2DPARAM param = static_cast<Graphics::ITexture2D*>(pTarget->GetResource().get())->GetParam();

  m_Size = create;
  m_TextureSize = param.Size;
  m_Format = param.Format;
  m_Area   = area;
}

//! 各種設定しておく
/*!
    @param  pTarget [i ]  要求があったときに差し出すターゲット
 */
void RenderTargetBase::Setup( const Graphics::SPRENDERTARGET& pTarget )
{
  MAID_ASSERT( pTarget->GetResource()->GetType()!=Graphics::IResource::TYPE_TEXTURE2D,
      "設定するリソースがテクスチャではありません" );
  const SIZE2DI size = static_cast<Graphics::ITexture2D*>(pTarget->GetResource().get())->GetParam().Size;

  Setup( pTarget, size, DRAWAREA_LEFTTOP );
}

}

