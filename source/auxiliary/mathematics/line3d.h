﻿/*!
    @file
    @brief	３次元直線テンプレート
 */



#ifndef maid2_auxiliary_mathematics_line3d_h
#define maid2_auxiliary_mathematics_line3d_h

#include"../../config/define.h"
#include"point3d.h"

namespace Maid
{ 
  /*!
      @brief	３次元直線を保持する構造体
   */
  template<typename TYPE>
  struct LINE3D_TEMPLATE
  {
    POINT3D_TEMPLATE<TYPE>	p0;		//!<	点１
    POINT3D_TEMPLATE<TYPE>	p1;		//!<	点２

    LINE3D_TEMPLATE(){}	//!<	コンストラクタ
    LINE3D_TEMPLATE( TYPE x0, TYPE y0, TYPE z0, TYPE x1, TYPE y1, TYPE z1 ) 	//!<	コンストラクタ
	    : p0(x0,y0,z0), p1(x1,y1,z0){}

    template<typename TYPE2>
    LINE3D_TEMPLATE( const LINE3D_TEMPLATE<TYPE2>& rha ) 	//!<	コンストラクタ
	    : p0(rha.p0), p1(rha.p1){}
  };

  template<typename TYPE>
  bool operator==( const LINE3D_TEMPLATE<TYPE>& lha, const LINE3D_TEMPLATE<TYPE>& rha )//!<	比較
  {
    return lha.p0==rha.p0 && lha.p1==rha.p1; 
  }
  template<typename TYPE>
  bool operator!=( const LINE3D_TEMPLATE<TYPE>& lha, const LINE3D_TEMPLATE<TYPE>& rha )//!<	比較
  {
    return !(lha==rha); 
  }

  //	直線同士の結合はできないと思うので + 演算子のオーバーロードは禁止
}

#endif
