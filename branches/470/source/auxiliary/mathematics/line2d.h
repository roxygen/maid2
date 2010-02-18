﻿/*!
    @file
    @brief	２次元直線テンプレート
  */



#ifndef maid2_auxiliary_mathematics_line2d_h
#define maid2_auxiliary_mathematics_line2d_h

#include"../../config/define.h"
#include"point2d.h"

namespace Maid
{ 
  /*!
      @brief	２次元直線を保持する構造体
   */
  template<typename TYPE>
  struct LINE2D_TEMPLATE
  {
    POINT2D_TEMPLATE<TYPE>	p0;		//!<	始点
    POINT2D_TEMPLATE<TYPE>	p1;		//!<	終点

    LINE2D_TEMPLATE(){}	//!<	コンストラクタ
    LINE2D_TEMPLATE( TYPE x0, TYPE y0, TYPE x1, TYPE y1 ) 	//!<	コンストラクタ
	    : p0(x0,y0), p1(x1,y1){}
    template<typename TYPE2>
    LINE2D_TEMPLATE( const LINE2D_TEMPLATE<TYPE2>& rha ) 	//!<	コンストラクタ
	    : p0(rha.p0), p1(rha.p1){}
  };

  template<typename TYPE>
  bool operator==( const LINE2D_TEMPLATE<TYPE>& lha, const LINE2D_TEMPLATE<TYPE>& rha )//!<	比較
  {
    return lha.p0==rha.p0 && lha.p1==rha.p1; 
  }
  template<typename TYPE>
  bool operator!=( const LINE2D_TEMPLATE<TYPE>& lha, const LINE2D_TEMPLATE<TYPE>& rha )//!<	比較
  {
    return !(lha==rha); 
  }

	//	直線同士の結合はできないと思うので + 演算子のオーバーロードは禁止
}

#endif
