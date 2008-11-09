/*!
    @file
    @brief	�R�����T�C�Y�e���v���[�g
 */



#ifndef maid2_auxiliary_mathematics_size3d_h
#define maid2_auxiliary_mathematics_size3d_h

#include"../../config/define.h"


namespace Maid
{ 
  /*!
      @brief	�����A�c��,������ێ�����\����
   */
  template<typename TYPE>
  struct SIZE3D_TEMPLATE
  {
    typedef SIZE3D_TEMPLATE<TYPE> SIZE;

    TYPE	w;		//!<	����
    TYPE	h;		//!<	�c��
    TYPE	d;		//!<	����

    SIZE3D_TEMPLATE(){}
    SIZE3D_TEMPLATE( TYPE _w, TYPE _h, TYPE _d ) 	//!<	�R���X�g���N�^
      : w(_w), h(_h),d(_d){}
    template<typename TYPE2>
    SIZE3D_TEMPLATE( const SIZE3D_TEMPLATE<TYPE2>& rha ) 	//!<	�R���X�g���N�^
    {
      w = (TYPE)rha.w;
      h = (TYPE)rha.h;
      d = (TYPE)rha.d;
    }

    //	��r���Z�q
    bool operator==( const SIZE& rha ) { return w==rha.w && h==rha.h && d==rha.d; }
    bool operator!=( const SIZE& rha ) { return !(*this==rha); }
	};
}

#endif
