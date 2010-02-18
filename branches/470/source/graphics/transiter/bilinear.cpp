﻿#include"bilinear.h"
#include"common.h"
#include"copy.h"

#include"../color.h"

//バイリニア補間はここがわかりやすいかな
// http://whiteforest.sakura.ne.jp/cg/i04.html
// http://www.daionet.gr.jp/~masa/column/98-06-27.html
// http://www7a.biglobe.ne.jp/~fairytale/article/program/graphics.html#bi_linear

namespace Maid
{
  namespace Transiter
  {

//  線形補間
static COLOR_R32G32B32A32F Leap( const COLOR_R32G32B32A32F& left, const COLOR_R32G32B32A32F& right, float pos )
{
  COLOR_R32G32B32A32F ret;

  const float inv = 1.0f-pos;

  ret.SetA( left.GetA()*inv   +   right.GetA()*pos );
  ret.SetR( left.GetR()*inv   +   right.GetR()*pos );
  ret.SetG( left.GetG()*inv   +   right.GetG()*pos );
  ret.SetB( left.GetB()*inv   +   right.GetB()*pos );

  return ret;
}

static COLOR_R32G32B32A32F BilinearW( const Surface& Src, float PosX, int y )
{
  int	left;
  int	right;
  float pos;
  FloatDiv( PosX, left, pos );
  right = left + 1;

  if( !(Src.GetSize().w < right) ) { return Src.GetPixel( POINT2DI(left ,y) ); }

  const COLOR_R32G32B32A32F cl = Src.GetPixel( POINT2DI(left ,y) );
  const COLOR_R32G32B32A32F cr = Src.GetPixel( POINT2DI(right,y) );

  return Leap(cl,cr,pos);
}

static COLOR_R32G32B32A32F BilinearWH( const Surface& Src, float PosX, float PosY )
{
  int	top;
  int	bottom;
  float pos;
  FloatDiv( PosY, top, pos );
  bottom = top + 1;

  if( !(Src.GetSize().h < bottom) ) { return BilinearW( Src, PosX, top ); }


  const COLOR_R32G32B32A32F ct = BilinearW( Src, PosX, top    );
  const COLOR_R32G32B32A32F cb = BilinearW( Src, PosX, bottom );

  return Leap(ct,cb,pos);
}

//! バイリニア補間でコピーする
/*!
    転送先は事前に作っておくこと

    @param  Src [i ]  転送元
    @param  Dst [ o]  転送先
 */
void Bilinear( const Surface& Src, Surface& Dst )
{
  RECT2DI src_rc(POINT2DI(0,0),Src.GetSize());
  RECT2DI dst_rc(POINT2DI(0,0),Dst.GetSize());

  Bilinear( Src, src_rc, Dst, dst_rc );
}


//! バイリニア補間でコピーする
/*!
    転送先は事前に作っておくこと

    @param  Src     [i ]  転送元
    @param  SrcRect [i ]  転送領域
    @param  Dst     [ o]  転送先
    @param  DstRect [i ]  転送領域
 */
void Bilinear( const Surface& Src, const RECT2DI& SrcRect, Surface& Dst, const RECT2DI& DstRect )
{
  const RECT2DI src_rc = Clip(Src,SrcRect);
  const RECT2DI dst_rc = Clip(Dst,DstRect);

  if( src_rc.w==dst_rc.w && src_rc.h==dst_rc.h )
  {
    Copy( Src, src_rc, Dst, dst_rc );
  }
  else
  {
    for( int posy=0; posy<dst_rc.h; ++posy )
    {
      const float y = (float(posy)+0.5f) / float(dst_rc.h);  //  この座標の色を求める
      const float src_y = y * float(src_rc.h) + src_rc.y;

      for( int posx=0; posx<dst_rc.w; ++posx )
      {
        const float x = (float(posx)+0.5f) / float(dst_rc.w);  //  この座標の色を求める
        const float src_x = x * float(src_rc.w) + src_rc.x;

        const COLOR_R32G32B32A32F c = BilinearWH( Src, src_x, src_y );

        Dst.SetPixel( POINT2DI(posx,posy), c );
      }
    }
  }
}


  }
}

