﻿/*!
    @file
    @brief	２次元座標テンプレート
 */



#ifndef maid2_auxiliary_mathematics_point2d_h
#define maid2_auxiliary_mathematics_point2d_h

#include"../../config/define.h"

namespace Maid
{ 
  /*!
      @brief	２次元座標を保持する構造体
   */
  template<typename TYPE>
  struct POINT2D_TEMPLATE
  {
    typedef POINT2D_TEMPLATE<TYPE> POINT;

    TYPE	x;		//!<	横
    TYPE	y;		//!<	縦


    POINT2D_TEMPLATE(){}	//!<	コンストラクタ
    POINT2D_TEMPLATE( TYPE _x, TYPE _y ) 	//!<	コンストラクタ
      : x(_x), y(_y){}

    template<typename TYPE2>
    POINT2D_TEMPLATE( const POINT2D_TEMPLATE<TYPE2>& rha ) 	//!<	コンストラクタ
    {
      x = (TYPE)rha.x;
      y = (TYPE)rha.y;
    }

    //	単項演算子
    POINT operator-() { return POINT(-x,-y); }

    //	比較演算子
    bool operator==( const POINT& rha ) { return x==rha.x && y==rha.y; }
    bool operator!=( const POINT& rha ) { return !(*this==rha); }

  };

  //	座標同士を足したり引いたりするのはおかしいので、
  //	二項演算子は用意しない VECTOR を使うこと
}

#endif
