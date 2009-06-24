﻿#include"common.h"
#include <math.h>

namespace Maid
{
  namespace Transiter
  {

    // rc を Src の範囲内でクリップする
RECT2DI Clip( const Surface& Src, const RECT2DI& rc )
{
  RECT2DI ret = rc;

  if( ret.x < 0 ) { ret.x = 0; }
  if( ret.y < 0 ) { ret.y = 0; }

  const int r = ret.GetRight();

  if( ret.x + Src.GetSize().w < ret.GetRight()  ) { ret.w = Src.GetSize().w - ret.x; }
  if( ret.y + Src.GetSize().h < ret.GetBottom() ) { ret.h = Src.GetSize().h - ret.y; }

  return ret;
}

    // src_rc を pos + Dst.getSize の範囲内でクリップする
RECT2DI Clip( const RECT2DI& src_rc, const Surface& Dst, const POINT2DI& pos )
{
  RECT2DI ret = src_rc;

  if( pos.x+Dst.GetSize().w < ret.w ) { ret.w = Dst.GetSize().w - pos.x; }
  if( pos.y+Dst.GetSize().h < ret.h ) { ret.h = Dst.GetSize().h - pos.y; }

  return ret;
}


//	Src を整数部と小数部に分ける
void FloatDiv( float Src, int& DstI, float& DstF )
{
	float i;
	DstF = modf( Src, &i );
	DstI = int(i+0.5f);
}

  }
}

