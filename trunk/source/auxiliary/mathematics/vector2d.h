/*!
    @file
    @brief	�Q�����x�N�g���e���v���[�g
 */



#ifndef maid2_auxiliary_mathematics_vector2d_h
#define maid2_auxiliary_mathematics_vector2d_h

#include"../../config/define.h"
#include"point2d.h"
#include"function.h"

namespace Maid
{ 
  /*!
      @brief	�Q�����x�N�g����ێ�����\����
   */
  template<typename TYPE>
  struct VECTOR2D_TEMPLATE
  { 
    typedef VECTOR2D_TEMPLATE<TYPE> VECTOR;
    typedef POINT2D_TEMPLATE<TYPE>  POINT;

    TYPE	x;		//!<	��
    TYPE	y;		//!<	�c

    VECTOR2D_TEMPLATE(){}		//!	�R���X�g���N�^
    VECTOR2D_TEMPLATE( TYPE _x, TYPE _y ): x(_x), y(_y){}
    VECTOR2D_TEMPLATE( const POINT& Start, const POINT& End ) { Reset(Start,End); }
    VECTOR2D_TEMPLATE( const POINT& pos ) { Reset(pos); }
  	
    //! ���K������
    /*!
    */
    const VECTOR& Normalize()
    {
	    const TYPE f = Math<TYPE>::sqrt(x*x+y*y);
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
    }

    //! ���W���x�N�g���ɕϊ�����
    /*!
 	      @param	pos [i ]	�x�N�g���̊J�n���W
     */
    void Reset( const POINT& pos )
    {
	    x = pos.x;
	    y = pos.y;
    }

    POINT ConvertPOINT2D() { return POINT(x,y); }

    //	�P�����Z�q
    VECTOR operator-() const{ return VECTOR(-x,-y); }

    //	��r���Z�q
    bool operator==( const VECTOR& rha ) const{ return x==rha.x && y==rha.y; }
    bool operator!=( const VECTOR& rha ) const{ return !(*this==rha); }

    //	�Đݒ�
    VECTOR& operator+=( const VECTOR& rha)	{ *this = *this+rha; return *this; }
    VECTOR& operator-=( const VECTOR& rha)	{ *this = *this-rha; return *this; }
    VECTOR& operator*=( const TYPE& val)	{ *this = *this*val; return *this; }
    VECTOR& operator/=( const TYPE& val)	{ *this = *this/val; return *this; }
  };


  //	�񍀉��Z�q
  template<typename TYPE>
  VECTOR2D_TEMPLATE<TYPE> operator+( const VECTOR2D_TEMPLATE<TYPE>& lha, const VECTOR2D_TEMPLATE<TYPE>& rha )
  {
    return VECTOR2D_TEMPLATE<TYPE>( lha.x+rha.x, lha.y+rha.y); 
  }
  template<typename TYPE>
  VECTOR2D_TEMPLATE<TYPE> operator-( const VECTOR2D_TEMPLATE<TYPE>& lha, const VECTOR2D_TEMPLATE<TYPE>& rha )
  {
    return VECTOR2D_TEMPLATE<TYPE>( lha.x-rha.x, lha.y-rha.y); 
  }
  template<typename TYPE>
  VECTOR2D_TEMPLATE<TYPE> operator*( const VECTOR2D_TEMPLATE<TYPE>& lha, const TYPE& rha )
  {
    return VECTOR2D_TEMPLATE<TYPE>( lha.x*rha, lha.y*rha); 
  }
  template<typename TYPE>
  VECTOR2D_TEMPLATE<TYPE> operator/( const VECTOR2D_TEMPLATE<TYPE>& lha, const TYPE& rha )
  {
    return VECTOR2D_TEMPLATE<TYPE>( lha.x/rha, lha.y/rha); 
  }

  //! ���ς����߂�
  /*!
      @param	lha		[i ]	�x�N�g�����̂P
      @param	rha		[i ]	�x�N�g�����̂Q
     
      @return	���ς̒l
   */
  template<typename TYPE>
  inline TYPE VectorDot( const VECTOR2D_TEMPLATE<TYPE>& lha, const VECTOR2D_TEMPLATE<TYPE>& rha )
  {
    return (lha.x*rha.x) + (lha.y*rha.y);
  }

  //! �O�ς����߂�
  /*!
      @param	lha		[i ]	�x�N�g�����̂P
      @param	rha		[i ]	�x�N�g�����̂Q
     
      @return	�O�ς̒l�i0���Ɠ����x�N�g��)
   */
  template<typename TYPE>
  inline TYPE VectorCross( const VECTOR2D_TEMPLATE<TYPE>& lha, const VECTOR2D_TEMPLATE<TYPE>& rha )
  {
    //  �Q���̊O�ςȂ�ĂȂ̂ŁA(x,y,0)�ɂ��ċ��߂�
    const VECTOR3D_TEMPLATE<TYPE> ret(
        (lha.y*    0) - (    0*rha.y),
        (    0*rha.x) - (lha.x*    0),
        (lha.x*rha.y) - (lha.y*rha.x)
      ); 

    return ret.z;
  }

  template<typename TYPE>
  inline VECTOR2D_TEMPLATE<TYPE> VectorRotate( const VECTOR2D_TEMPLATE<TYPE>& lha, float rad )
  {
    /*
      �s��v�Z��
      |x,y|| cos, sin|
           |-sin, cos|
      x' = x * cos�� - y * sin��
      y' = x * sin�� + y * cos��
      �ł��B
      ���ʂɐ}��`���āA���@�藝���g���΂킩�邩�ƁB
    */

    const TYPE cos = Math<TYPE>::cos(rad);
    const TYPE sin = Math<TYPE>::sin(rad);

    const TYPE x = lha.x * cos - lha.y * sin;
    const TYPE y = lha.x * sin + lha.y * cos;

    return VECTOR2D_TEMPLATE<TYPE>(x,y);
  }
}

//	Windows �̂Ƃ��̂� DXLIB ���g�����o�[�W�����𑶍݂�����
#ifdef USE_DIRECT3DX9
  #include"../../config/win32.h"
  #include<d3dx9.h>

  namespace Maid
  {
    namespace d3dxvector2d
    {
      typedef D3DXVECTOR2* (WINAPI *NORMALIZE)(D3DXVECTOR2*,CONST D3DXVECTOR2*);
      typedef FLOAT (WINAPI *DOT)(CONST D3DXVECTOR2*,CONST D3DXVECTOR2*);
      typedef FLOAT (WINAPI *CCW)(CONST D3DXVECTOR2*,CONST D3DXVECTOR2*);
      extern NORMALIZE Normalize;
      extern DOT Dot;
      extern CCW ccw;

      void Initialize( HMODULE hModule );
      void Finalize();
    }

    const VECTOR2D_TEMPLATE<FLOAT>& VECTOR2D_TEMPLATE<FLOAT>::Normalize()
    {
      d3dxvector2d::Normalize( (D3DXVECTOR2*)this, (D3DXVECTOR2*)this );
      return *this;
    }

    inline FLOAT VectorDot( const VECTOR2D_TEMPLATE<FLOAT>& lha, const VECTOR2D_TEMPLATE<FLOAT>& rha )
    {
      return d3dxvector2d::Dot( (D3DXVECTOR2*)&lha, (D3DXVECTOR2*)&rha );
    }

    inline FLOAT VectorCross( const VECTOR2D_TEMPLATE<FLOAT>& lha, const VECTOR2D_TEMPLATE<FLOAT>& rha )
    {
      return d3dxvector2d::ccw( (D3DXVECTOR2*)&lha, (D3DXVECTOR2*)&rha );
    }
  }
#endif

#endif
