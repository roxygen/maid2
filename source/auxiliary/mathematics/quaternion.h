/*!
    @file
    @brief	�N�H�[�^�j�I���N���X
 */

#ifndef maid2_auxiliary_mathematics_quaternion_h
#define maid2_auxiliary_mathematics_quaternion_h

#include"../../config/define.h"

#include"matrix4d.h"
#include"vector3d.h"
#include"function.h"

namespace Maid
{ 
  /*!
     @brief �N�H�[�^�j�I����ێ�����\����
   */
  template<typename TYPE>
  struct QUATERNION_TEMPLATE
  {
    typedef VECTOR3D_TEMPLATE<TYPE> VECTOR;
    typedef MATRIX4D_TEMPLATE<TYPE> MATRIX;
    typedef QUATERNION_TEMPLATE<TYPE> QUATERNION;

    TYPE i;
    TYPE j;
    TYPE k;
    TYPE w;

    QUATERNION_TEMPLATE(){}
    QUATERNION_TEMPLATE( TYPE _i, TYPE _j, TYPE _k, TYPE _w ) :i(_i),j(_j),k(_k),w(_w){  }
    QUATERNION_TEMPLATE( const VECTOR& axis, TYPE angle )
    {
      SetRotation( axis, angle );
    }
    QUATERNION_TEMPLATE( const VECTOR& v0,const VECTOR& v1 )
    {
      SetRotationArc( v0, v1 );
    }

    //! �N�H�[�^�j�I����ݒ肷��
    /*!
       @param	axis	[i ]	��]���i���K�����Ă������Ɓj
       @param	angle	[i ]	��]�ʁi���W�A���j
     */
    const QUATERNION& SetRotation( const VECTOR& axis, TYPE angle )
    {
    //	DEN_ASSERT( v0.x*v0.x+v0.y*v0.y+v0.z*v0.z!=0, "���K������Ă��܂���" );

      const TYPE s = Math<TYPE>::sin(angle/2);

      w = Math<TYPE>::cos(angle/2);
      i = s*axis.x;
      j = s*axis.y;
      k = s*axis.z;
      return *this;
    }

    //! ���鎲���炠�鎲�։�]����N�H�[�^�j�I����ݒ肷��
    /*!
        @param	v0	[i ]	���鎲�P�i���K�����Ă������Ɓj
        @param	v1	[i ]	���鎲�Q�i���K�����Ă������Ɓj
     */
    const QUATERNION& SetRotationArc(const VECTOR& v0,const VECTOR& v1)
    {
    //	DEN_ASSERT( v0.x*v0.x+v0.y*v0.y+v0.z*v0.z!=0, "���K������Ă��܂���" );
    //	DEN_ASSERT( v1.x*v1.x+v1.y*v1.y+v1.z*v1.z!=0, "���K������Ă��܂���" );

      const VECTOR c = VectorCross(v0,v1);
      const TYPE   d = VectorDot(v0,v1);
      const TYPE   s = (TYPE)sqrt((1+d)*2);
      i = c.x / s;
      j = c.y / s;
      k = c.z / s;
      w = s /2.0f;
      return *this;
    }
    //! ���K������
    /*!
    */
    const QUATERNION& Normalize()
    {
      TYPE n = Math<TYPE>::sqrt(GetNorm());

      i /= n;
      j /= n;
      k /= n;
      w /= n;

      return *this;
    }

    //! ���������߂�
    /*!
    */
    QUATERNION GetConjugate() const
    {
      return QUATERNION( -i, -j, -k, w );
    }
    //! �t�N�H�[�^�j�I�������߂�
    /*!
    */
    QUATERNION GetInverse() const
    {
      QUATERNION  r( GetConjugate() );
      const TYPE n = GetNorm();

      r.i /= n;
      r.j /= n;
      r.k /= n;
      r.w /= n;

      return r;
    }


    //! �m���������߂�
    /*!
    */
    TYPE GetNorm() const
    {
      return i*i + j*j + k*k + w*w;
    }

    //! ���̃N�H�[�^�j�I���Ɠ�����]��\���}�g���N�X���쐬����
    /*!
        @param	v	[i ]	���łɐݒ肷�镽�s�ړ��ʁi�f�t�H���g��0,0,0�j
     */
    MATRIX GetMatrix4D( const VECTOR& v=VECTOR(0,0,0)) const
    {
      return MATRIX
	      (
		      1-2*(j*j+k*k),   2*(i*j+w*k),   2*(i*k-w*j), 0,
		        2*(i*j-w*k), 1-2*(i*i+k*k),   2*(j*k+w*i), 0,
		        2*(i*k+w*j),   2*(j*k-w*i), 1-2*(i*i+j*j), 0,
		                v.x,           v.y,           v.z, 1
	      );
    }

  };


  template<typename TYPE>
  bool operator==( const QUATERNION_TEMPLATE<TYPE>& lha, const QUATERNION_TEMPLATE<TYPE>& rha )//!<	��r
  {
    return lha.i==rha.i && lha.j==rha.j && lha.k==rha.k && lha.w==rha.w; 
  }
  template<typename TYPE>
  bool operator!=( const QUATERNION_TEMPLATE<TYPE>& lha, const QUATERNION_TEMPLATE<TYPE>& rha )//!<	��r
  {
    return !(lha==rha); 
  }

  template<typename TYPE>
  QUATERNION_TEMPLATE<TYPE> operator+( const QUATERNION_TEMPLATE<TYPE>& lha, const QUATERNION_TEMPLATE<TYPE>& rha )
  {
    QUATERNION_TEMPLATE<TYPE> r;
    r.i = lha.i + rha.i;
    r.j = lha.j + rha.j;
    r.k = lha.k + rha.k;
    r.w = lha.w + rha.w;

    return r;
  }
  template<typename TYPE>
  QUATERNION_TEMPLATE<TYPE> operator-( const QUATERNION_TEMPLATE<TYPE>& lha, const QUATERNION_TEMPLATE<TYPE>& rha )
  {
    QUATERNION_TEMPLATE<TYPE> r;
    r.i = lha.i - rha.i;
    r.j = lha.j - rha.j;
    r.k = lha.k - rha.k;
    r.w = lha.w - rha.w;

    return r;
  }
  template<typename TYPE>
  QUATERNION_TEMPLATE<TYPE> operator*( const QUATERNION_TEMPLATE<TYPE>& lha, const QUATERNION_TEMPLATE<TYPE>& rha )
  {
    QUATERNION_TEMPLATE<TYPE> r;

    r.i = lha.w*rha.i + lha.x*rha.w + lha.y*rha.k - lha.z*rha.j; 
    r.j = lha.w*rha.j - lha.x*rha.k + lha.y*rha.w + lha.z*rha.i; 
    r.k = lha.w*rha.k + lha.x*rha.j - lha.y*rha.i + lha.z*rha.w; 
    r.w = lha.w*rha.w - lha.x*rha.i - lha.y*rha.j - lha.z*rha.k; 

    return r;
  }

}
	//	Windows �̂Ƃ��̂� DXLIB ���g�����o�[�W�����𑶍݂�����
#ifdef USE_DIRECT3DX9
  #include"../../config/win32.h"
  #include<d3dx9.h>

  namespace Maid
  {
    namespace d3dxquaternion
    {
      typedef D3DXQUATERNION* (WINAPI *ROTATIONAXIS)(D3DXQUATERNION*,CONST D3DXVECTOR3*,FLOAT);
      typedef D3DXQUATERNION* (WINAPI *NORMALIZE)(D3DXQUATERNION*,CONST D3DXQUATERNION*);
      typedef D3DXQUATERNION* (WINAPI *INVERSE)(D3DXQUATERNION*,CONST D3DXQUATERNION*);
      typedef D3DXMATRIX * (WINAPI *MATRIXROTATIONQUATERNION)(D3DXMATRIX *,CONST D3DXQUATERNION*);
      typedef D3DXQUATERNION* (WINAPI *MULTIPLY)(D3DXQUATERNION*,CONST D3DXQUATERNION*,CONST D3DXQUATERNION*);

      extern ROTATIONAXIS RotationAxis;
      extern NORMALIZE Normalize;
      extern INVERSE Inverse;
      extern MATRIXROTATIONQUATERNION MatrixRotationQuaternion;
      extern MULTIPLY Multiply;

      void Initialize( HMODULE hModule );
      void Finalize();
    }


    const QUATERNION_TEMPLATE<FLOAT>& QUATERNION_TEMPLATE<FLOAT>::SetRotation( const VECTOR3D_TEMPLATE<FLOAT>& axis, FLOAT angle )
    {
      d3dxquaternion::RotationAxis( (D3DXQUATERNION*)this, (const D3DXVECTOR3*)&axis, angle );
	    return *this;
    }
    const QUATERNION_TEMPLATE<FLOAT>& QUATERNION_TEMPLATE<FLOAT>::Normalize()
    {
	    d3dxquaternion::Normalize( (D3DXQUATERNION*)this, (D3DXQUATERNION*)this );
	    return *this;
    }

    QUATERNION_TEMPLATE<FLOAT> QUATERNION_TEMPLATE<FLOAT>::GetInverse() const
    {
	    QUATERNION_TEMPLATE<FLOAT> r;
	    d3dxquaternion::Inverse( (D3DXQUATERNION*)&r, (D3DXQUATERNION*)this );
	    return r;
    }


    MATRIX4D_TEMPLATE<FLOAT> QUATERNION_TEMPLATE<FLOAT>::GetMatrix4D( const VECTOR3D_TEMPLATE<FLOAT>& v) const
    {
	    MATRIX4D_TEMPLATE<FLOAT> tmp;
	    d3dxquaternion::MatrixRotationQuaternion( (D3DXMATRIX*)&tmp, (D3DXQUATERNION*)this );
	    tmp.m41 = v.x;
	    tmp.m42 = v.y;
	    tmp.m43 = v.z;
	    return tmp;
    }

    inline QUATERNION_TEMPLATE<FLOAT> operator*( const QUATERNION_TEMPLATE<FLOAT>& lha, const QUATERNION_TEMPLATE<FLOAT>& rha )
    {
	    QUATERNION_TEMPLATE<FLOAT> r;
	    d3dxquaternion::Multiply( (D3DXQUATERNION*)&r, (D3DXQUATERNION*)&lha, (D3DXQUATERNION*)&rha );
	    return r;
    }
}

	#endif


#endif