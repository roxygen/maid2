/*!
 	  @file
 	  @brief	�S�����x�N�g���e���v���[�g
 */



#ifndef maid2_auxiliary_mathematics_vector4d_h
#define maid2_auxiliary_mathematics_vector4d_h

#include"../../config/define.h"
#include"point4d.h"
#include"function.h"


namespace Maid
{ 
  /*!
      @brief	�S�����x�N�g����ێ�����\����
   */
  template<typename TYPE>
  struct VECTOR4D_TEMPLATE
  { 
    typedef VECTOR4D_TEMPLATE<TYPE> VECTOR;
    typedef POINT4D_TEMPLATE<TYPE>  POINT;

    TYPE	x;		//!<	��
    TYPE	y;		//!<	�c
    TYPE	z;		//!<	��
    TYPE	w;		//!<	w?

    VECTOR4D_TEMPLATE(){}
    //!	�R���X�g���N�^
    VECTOR4D_TEMPLATE( TYPE _x, TYPE _y, TYPE _z, TYPE _w ): x(_x), y(_y), z(_z), w(_w){}
    VECTOR4D_TEMPLATE( const POINT& Start, const POINT& End ) {	Reset(Start,End); }
  	

    //! ���K������
    /*!
    */
    const VECTOR& Normalize()
    {
      const TYPE f = Math<TYPE>::sqrt(x*x+y*y+z*z+w*w);
      *this /= f;
      return *this;
    }

    //! ���W���x�N�g���ɕϊ�����
    /*!
        @param	Start [i ]	�x�N�g���̊J�n���W
        @param	End   [i ]	�x�N�g���̏I�����W
     */
    void Reset( const POINT& Start, const POINT& End )
    {
      x = End.x-Start.x;
      y = End.y-Start.y;
      z = End.z-Start.z;
      w = End.w-Start.w;
    }
    //! ���W���x�N�g���ɕϊ�����
    /*!
        @param	pos [i ]	�x�N�g���̊J�n���W
     */
    void Reset( const POINT& pos )
    {
      x = pos.x;
      y = pos.y;
      z = pos.z;
      w = pos.w;
    }

    //	�P�����Z�q
    VECTOR operator-() const{ return VECTOR(-x,-y,-z,-w); }

    //	��r���Z�q
    bool operator==( const VECTOR& rha ) const{ return x==rha.x && y==rha.y && z==rha.z && w==rha.w; }
    bool operator!=( const VECTOR& rha ) const{ return !(*this==rha); }

    //	�Đݒ�
    VECTOR& operator+=( const VECTOR& rha)	{ *this = *this+rha; return *this; }
    VECTOR& operator-=( const VECTOR& rha)	{ *this = *this-rha; return *this; }
    VECTOR& operator*=( const TYPE& val)	{ *this = *this*val; return *this; }
    VECTOR& operator/=( const TYPE& val)	{ *this = *this/val; return *this; }

  };

  //	�񍀉��Z�q
  template<typename TYPE>
  VECTOR4D_TEMPLATE<TYPE> operator-( const VECTOR4D_TEMPLATE<TYPE>& lha, const VECTOR4D_TEMPLATE<TYPE>& rha )
  {
    return VECTOR4D_TEMPLATE<TYPE>( lha.x-rha.x, lha.y-rha.y, lha.z-rha.z, lha.w-rha.w); 
  }
  template<typename TYPE>
  VECTOR4D_TEMPLATE<TYPE> operator+( const VECTOR4D_TEMPLATE<TYPE>& lha, const VECTOR4D_TEMPLATE<TYPE>& rha )
  {
    return VECTOR4D_TEMPLATE<TYPE>( lha.x+rha.x, lha.y+rha.y, lha.z+rha.z, lha.w+rha.w); 
  }
  template<typename TYPE>
  VECTOR4D_TEMPLATE<TYPE> operator*( const VECTOR4D_TEMPLATE<TYPE>& lha, const TYPE& rha )
  {
    return VECTOR4D_TEMPLATE<TYPE>( lha.x*rha, lha.y*rha, lha.z*rha, lha.w*rha); 
  }
  template<typename TYPE>
  VECTOR4D_TEMPLATE<TYPE> operator/( const VECTOR4D_TEMPLATE<TYPE>& lha, const TYPE& rha )
  {
    return VECTOR4D_TEMPLATE<TYPE>( lha.x/rha, lha.y/rha, lha.z/rha, lha.w/rha); 
  }

  //! ���ς����߂�
  /*!
      @param	lha		[i ]	�x�N�g�����̂P
      @param	rha		[i ]	�x�N�g�����̂Q
     
      @return	���ς̒l
   */
  template<typename TYPE>
  inline TYPE VectorDot( const VECTOR4D_TEMPLATE<TYPE>& lha, const VECTOR4D_TEMPLATE<TYPE>& rha )
  {
    return (lha.x*rha.x) + (lha.y*rha.y) + (lha.z*rha.z) + (lha.w*rha.w);
  }


}
	//	Windows �̂Ƃ��̂� DXLIB ���g�����o�[�W�����𑶍݂�����
  #ifdef USE_DIRECT3DX9
  #include"../../config/win32.h"
    #include<d3dx9.h>
    #pragma comment( lib, "d3dx9.lib" )

    namespace Maid
    {
      const VECTOR4D_TEMPLATE<FLOAT>& VECTOR4D_TEMPLATE<FLOAT>::Normalize()
      {
	      D3DXVec4Normalize( (D3DXVECTOR4*)this, (D3DXVECTOR4*)this );
	      return *this;
      }

      inline FLOAT VectorDot( const VECTOR4D_TEMPLATE<FLOAT>& lha, const VECTOR4D_TEMPLATE<FLOAT>& rha )
      {
	      return D3DXVec4Dot( (D3DXVECTOR4*)&lha, (D3DXVECTOR4*)&rha );
      }
    }
  #endif


#endif
