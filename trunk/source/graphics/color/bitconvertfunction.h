/*!
    @file
    @brief	�r�b�g���̕ϊ����s���e�[�u��
 */
#ifndef maid2_graphics_color_bitconvertfunction_h
#define maid2_graphics_color_bitconvertfunction_h

#include"../../config/define.h"
#include"../../config/typedef.h"

namespace Maid
{
  /*
      �֐��̌���
      XXAtoYYB �ƂȂ��Ă���
      XX,YY���r�b�g��
      A,B ��I=���� F=����
      �ɂȂ��Ă܂�
  */


  unt08 BITCONVERT01Ito08I( unt08 s );
  unt08 BITCONVERT02Ito08I( unt08 s );
  unt08 BITCONVERT04Ito08I( unt08 s );
  unt08 BITCONVERT05Ito08I( unt08 s );
  unt08 BITCONVERT06Ito08I( unt08 s );

  unt16 BITCONVERT01Ito16I( unt08 s );
  unt16 BITCONVERT02Ito16I( unt08 s );
  unt16 BITCONVERT04Ito16I( unt08 s );
  unt16 BITCONVERT05Ito16I( unt08 s );
  unt16 BITCONVERT06Ito16I( unt08 s );
  unt16 BITCONVERT08Ito16I( unt08 s );
  unt16 BITCONVERT10Ito16I( unt16 s );


  unt08 BITCONVERT08Ito01I( unt08 s );
  unt08 BITCONVERT08Ito02I( unt08 s );
  unt08 BITCONVERT08Ito04I( unt08 s );
  unt08 BITCONVERT08Ito05I( unt08 s );
  unt08 BITCONVERT08Ito06I( unt08 s );

  unt08 BITCONVERT16Ito01I( unt16 s );
  unt08 BITCONVERT16Ito02I( unt16 s );
  unt08 BITCONVERT16Ito04I( unt16 s );
  unt08 BITCONVERT16Ito05I( unt16 s );
  unt08 BITCONVERT16Ito06I( unt16 s );
  unt08 BITCONVERT16Ito08I( unt16 s );
  unt16 BITCONVERT16Ito10I( unt16 s );
}

#include"bitconvertfunction-inl.h"

#endif
