/*!
    @file
    @brief	�R������`�e���v���[�g
 */

#ifndef maid2_auxiliary_mathematics_rect3d_h
#define maid2_auxiliary_mathematics_rect3d_h

#include"../../config/define.h"

#include"point3d.h"
#include"size3d.h"

namespace Maid
{ 
  /*!
      @brief	�����̂�����킷�\����
   */
  template<typename TYPE>
  struct RECT3D_TEMPLATE
  {
    TYPE x;	//	��_
    TYPE y;
    TYPE z;

    TYPE w;	//	��
    TYPE h;
    TYPE d;

    RECT3D_TEMPLATE(){}
    RECT3D_TEMPLATE( TYPE _x, TYPE _y, TYPE _z, TYPE _w, TYPE _h, TYPE _d ) //!<	�R���X�g���N�^
      : x(_x),y(_y),z(_z),w(_w),h(_h),d(_d)
    {
    }	

    template<typename TYPE2>
    RECT3D_TEMPLATE( const RECT3D_TEMPLATE<TYPE2>& rha ) 	//!<	�R���X�g���N�^
    {
      x = (TYPE)rha.x;
      y = (TYPE)rha.y;
      z = (TYPE)rha.z;
      w = (TYPE)rha.w;
      h = (TYPE)rha.h;
      d = (TYPE)rha.d;
    }

    TYPE GetRight() const  { return x + w; }
    TYPE GetBottom()const { return y + h; }
    TYPE GetBack()	const { return z + d; }

    POINT3D_TEMPLATE<TYPE> GetPoint()const{ return POINT3D_TEMPLATE<TYPE>(x,y,z); }
    SIZE3D_TEMPLATE<TYPE>  GetSize() const{ return SIZE3D_TEMPLATE<TYPE>(w,h,d); }
  };

}
#endif
