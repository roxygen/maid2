﻿#ifndef maid2_graphics_transiter_common_h
#define maid2_graphics_transiter_common_h

#include"../../config/define.h"
#include"../../auxiliary/mathematics/rect.h"
#include"../../auxiliary/mathematics/point.h"
#include"../surface.h"


namespace Maid
{
  namespace Transiter
  {
    RECT2DI Clip( const Surface& Src, const RECT2DI& rc );
    RECT2DI Clip( const RECT2DI& src_rc, const Surface& Dst, const POINT2DI& pos );

    void FloatDiv( float Src, int& DstI, float& DstF ); 
  }
}

#endif
