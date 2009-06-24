﻿/*!
    @file
    @brief	４次元座標テンプレート
 */



#ifndef maid2_auxiliary_mathematics_point4d_h
#define maid2_auxiliary_mathematics_point4d_h

#include"../../config/define.h"

namespace Maid
{ 
  /*!
      @brief	４次元座標を保持する構造体
   */
  template<typename TYPE>
  struct POINT4D_TEMPLATE
  {
    typedef POINT2D_TEMPLATE<TYPE> POINT;

    TYPE	x;		//!<	横
    TYPE	y;		//!<	縦
    TYPE	z;		//!<	奥
    TYPE	w;		//!<	？

    POINT4D_TEMPLATE(){}	//!<	コンストラクタ
    POINT4D_TEMPLATE( TYPE _x, TYPE _y, TYPE _z, TYPE _w ) 	//!<	コンストラクタ
      : x(_x), y(_y), z(_z), w(_w){}
    template<typename TYPE2>
    POINT4D_TEMPLATE( const POINT4D_TEMPLATE<TYPE2>& rha ) 	//!<	コンストラクタ
    {
      x = (TYPE)rha.x;
      y = (TYPE)rha.y;
      z = (TYPE)rha.z;
      w = (TYPE)rha.w;
    }

    //	単項演算子
    POINT operator-() { return POINT(-x,-y,-z,-w); }

    //	比較演算子
    bool operator==( const POINT& rha ) { return x==rha.x && y==rha.y && z==rha.z && w==rha.w; }
    bool operator!=( const POINT& rha ) { return !(*this==rha); }

  };

  //	座標同士を足したり引いたりするのはおかしいので、
  //	二項演算子は用意しない VECTOR を使うこと
}

#endif
