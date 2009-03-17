/*!
    @file
    @brief	�R�����x�N�g���e���v���[�g
 */



#ifndef maid2_auxiliary_mathematics_vector3d_h
#define maid2_auxiliary_mathematics_vector3d_h

#include"../../config/define.h"
#include"point3d.h"
#include"vector4d.h"
#include"function.h"


namespace Maid
{ 
  /*!
      @brief	�R�����x�N�g����ێ�����\����
   */
  template<typename TYPE>
  struct VECTOR3D_TEMPLATE
  { 
    typedef VECTOR3D_TEMPLATE<TYPE> VECTOR;
    typedef POINT3D_TEMPLATE<TYPE>  POINT;

    TYPE	x;		//!<	��
    TYPE	y;		//!<	�c
    TYPE	z;		//!<	��

    VECTOR3D_TEMPLATE(){}
    VECTOR3D_TEMPLATE( TYPE _x, TYPE _y, TYPE _z ): x(_x), y(_y), z(_z){}
    VECTOR3D_TEMPLATE( const POINT& Start, const POINT& End ) { Reset(Start,End); }

    //! ���K������
    /*!
    */
    const VECTOR& Normalize()
    {
      const TYPE f = Math<TYPE>::sqrt(x*x+y*y+z*z);
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
    }

    VECTOR4D_TEMPLATE<TYPE> Convert4D()const
    {
      return VECTOR4D_TEMPLATE<TYPE>(x,y,z,1);
    }

    //	�P�����Z�q
    VECTOR operator-() const{ return VECTOR(-x,-y,-z); }

    //	��r���Z�q
    bool operator==( const VECTOR& rha ) const{ return x==rha.x && y==rha.y && z==rha.z; }
    bool operator!=( const VECTOR& rha ) const{ return !(*this==rha); }

    //	�Đݒ�
    VECTOR& operator+=( const VECTOR& rha)	{ *this = *this+rha; return *this; }
    VECTOR& operator-=( const VECTOR& rha)	{ *this = *this-rha; return *this; }
    VECTOR& operator*=( const TYPE& val)	{ *this = *this*val; return *this; }
    VECTOR& operator/=( const TYPE& val)	{ *this = *this/val; return *this; }
  };


  //	�񍀉��Z�q
  template<typename TYPE>
  VECTOR3D_TEMPLATE<TYPE> operator+( const VECTOR3D_TEMPLATE<TYPE>& lha, const VECTOR3D_TEMPLATE<TYPE>& rha )
  {
    return VECTOR3D_TEMPLATE<TYPE>( lha.x+rha.x, lha.y+rha.y, lha.z+rha.z); 
  }
  template<typename TYPE>
  VECTOR3D_TEMPLATE<TYPE> operator-( const VECTOR3D_TEMPLATE<TYPE>& lha, const VECTOR3D_TEMPLATE<TYPE>& rha )
  {
    return VECTOR3D_TEMPLATE<TYPE>( lha.x-rha.x, lha.y-rha.y, lha.z-rha.z); 
  }
  template<typename TYPE>
  VECTOR3D_TEMPLATE<TYPE> operator*( const VECTOR3D_TEMPLATE<TYPE>& lha, const TYPE& rha )
  {
    return VECTOR3D_TEMPLATE<TYPE>( lha.x*rha, lha.y*rha, lha.z*rha); 
  }
  template<typename TYPE>
  VECTOR3D_TEMPLATE<TYPE> operator/( const VECTOR3D_TEMPLATE<TYPE>& lha, const TYPE& rha )
  {
    return VECTOR3D_TEMPLATE<TYPE>( lha.x/rha, lha.y/rha, lha.z/rha); 
  }

  //! ���ς����߂�
  /*!
      @param	lha		[i ]	�x�N�g�����̂P
      @param	rha		[i ]	�x�N�g�����̂Q
     
      @return	���ς̒l
   */
  template<typename TYPE>
  inline TYPE VectorDot( const VECTOR3D_TEMPLATE<TYPE>& lha, const VECTOR3D_TEMPLATE<TYPE>& rha )
  {
    return (lha.x*rha.x) + (lha.y*rha.y) + (lha.z*rha.z);
  }

  //! �O�ς����߂�
  /*!
      ������W�n�ł̌v�Z�ł�
      @param	lha		[i ]	�x�N�g�����̂P
      @param	rha		[i ]	�x�N�g�����̂Q
     
      @return	�O�ς̃x�N�g��
   */
  template<typename TYPE>
  inline VECTOR3D_TEMPLATE<TYPE> VectorCross( const VECTOR3D_TEMPLATE<TYPE>& lha, const VECTOR3D_TEMPLATE<TYPE>& rha )
  {
    return VECTOR3D_TEMPLATE<TYPE>(
        (lha.y*rha.z) - (lha.z*rha.y),
        (lha.z*rha.x) - (lha.x*rha.z),
        (lha.x*rha.y) - (lha.y*rha.x)
      ); 
  }


}
	//	Windows �̂Ƃ��̂� DXLIB ���g�����o�[�W�����𑶍݂�����
#ifdef USE_DIRECT3DX9
  #include"../../config/win32.h"
  #include<d3dx9.h>

  namespace Maid
  {
    namespace d3dxvector3d
    {
      typedef D3DXVECTOR3* (WINAPI *NORMALIZE)(D3DXVECTOR3*,CONST D3DXVECTOR3*);
      extern NORMALIZE Normalize;

      void Initialize( HMODULE hModule );
      void Finalize();
    }

    const VECTOR3D_TEMPLATE<FLOAT>& VECTOR3D_TEMPLATE<FLOAT>::Normalize()
    {
      d3dxvector3d::Normalize( (D3DXVECTOR3*)this, (D3DXVECTOR3*)this );
      return *this;
    }
  }
#endif


#endif
