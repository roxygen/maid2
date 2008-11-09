/*!
    @file
    @brief	�Q�����T�C�Y�e���v���[�g
 */



#ifndef maid2_auxiliary_mathematics_size2d_h
#define maid2_auxiliary_mathematics_size2d_h

#include"../../config/define.h"

namespace Maid
{ 
  /*!
      @brief	�����A�c����ێ�����\����
   */
  template<typename TYPE>
  struct SIZE2D_TEMPLATE
  {
    typedef SIZE2D_TEMPLATE<TYPE> SIZE;

    TYPE	w;		//!<	����
    TYPE	h;		//!<	�c��

    SIZE2D_TEMPLATE(){}				 		        //!<	�R���X�g���N�^
    SIZE2D_TEMPLATE( TYPE _w, TYPE _h )   //!<	�R���X�g���N�^
      : w(_w), h(_h){}
    template<typename TYPE2>
    SIZE2D_TEMPLATE( const SIZE2D_TEMPLATE<TYPE2>& rha ) 	//!<	�R���X�g���N�^
    {
      w = (TYPE)rha.w;
      h = (TYPE)rha.h;
    }

    //	��r���Z�q
    bool operator==( const SIZE& rha ) { return w==rha.w && h==rha.h; }
    bool operator!=( const SIZE& rha ) { return !(*this==rha); }
	};
}

#endif
