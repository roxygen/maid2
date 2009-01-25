#include"camera.h"
#include"../auxiliary/collision.h"


namespace Maid
{

/*!
    @class	Camera camera.h
    @brief	�J�����N���X
\n          �r���[�}�g���N�X�A�v���W�F�N�V�����}�g���N�X���Ǘ�����
 */

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! �R���X�g���N�^
/*!
 *
 */
Camera::Camera()
  :m_IsCalcViewMatrix(false)
{
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! �f�X�g���N�^
/*!
 *
 */
Camera::~Camera()
{
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! �����ˉe�}�g���N�X�̍쐬
/*!
    @param  Fov     [i ]	�J�����̎���p
    @param  Aspect  [i ]	�X�N���[���̃A�X�y�N�g��(���ʂ�Width/Height)
    @param  Near    [i ]	�N���b�v�����O�̋���
    @param  Far     [i ]	�N���b�v���鉜�̋���
 */
void Camera::SetPerspective( float Fov, float Aspect, float Near, float Far )
{
  m_ProjectionMatrix.SetPerspective(Fov,Aspect,Near,Far );
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ���ˉe�}�g���N�X�̍쐬
/*!
    @param  Fov     [i ]  �J�����̎���p
    @param  Aspect  [i ]  �X�N���[���̃A�X�y�N�g��
    @param  Near    [i ]  �N���b�v�����O�̋���
    @param  Far     [i ]  �N���b�v���鉜�̋���
    @param  ProjectionDepth [i ]  ���ۂɎˉe����ʒu�B�J�������猩�����W�͂��ׂ�
                                    (x,y,ProjectionDepth) �ɂ�����̂Ƃ��ĕ`�悳���
 */
void Camera::SetOrthogonal( float Fov, float Aspect, float Near, float Far, float ProjectionDepth )
{
  MATRIX4DF m;

  m.SetOrthogonal( Fov, Aspect, ProjectionDepth, Far );

  const float CenterD = (ProjectionDepth-Near) / (Far - Near);
  m_ProjectionMatrix = m * MATRIX4DF().SetTranslate(0,0,CenterD);
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! �R�c��Ԓ��łQ�c�\���ɕ֗��ȃJ�����ݒ������
/*!
    �_(0,                      0,0)�@���X�N���[�����W��(0,0) ��
\n  �_(0,          -ScreenHeight,0)�@���X�N���[�����W��(0,ScreenHeight) ��
\n  �_(ScreenWidth,-ScreenHeight,0)�@���X�N���[�����W��(ScreenWidth,ScreenHeight) ��
\n  �Ȃ�悤�ɃJ�������W���v�Z����܂�
 
    @param  ScreenWidth   [i ]  ��ʉ��T�C�Y
    @param  ScreenHeight  [i ]  ��ʏc�T�C�Y
    @param  Near          [i ]  ��ʎ�O�̋���
    @param  Far           [i ]  ��ʉ��̋���
 */
void Camera::Set2DPosition( int ScreenWidth, int ScreenHeight, float Near, float Far )
{
  const float Width  = float(ScreenWidth /2);
  const float Height = float(ScreenHeight/2);
  const float Fovy   = DEGtoRAD(60.0f);
  const float Depth  = Height/Math<float>::tan(Fovy/2.0f);

  const POINT3DF DefaultPosition( Width, Height, -Depth );
  const POINT3DF DefaultTarget  ( Width, Height,      0 );

  SetPerspective( Fovy, Width/Height, Near, Far );
  SetPosition( DefaultPosition );
  SetTarget  ( DefaultTarget );
  SetUpVector( VECTOR3DF( 0, 1, 0) );
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! �J�����̂�����W�̐ݒ�
/*!
    @param  pos [i ]  �V�����ݒ肷����W
 */
void Camera::SetPosition( const POINT3DF&  pos )
{
  m_Pos = pos;
  m_IsCalcViewMatrix = false;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! �J�����̒������W�̐ݒ�
/*!
  @param  pos [i ]  �V�����ݒ肷����W
 */
void Camera::SetTarget  ( const POINT3DF&  pos )
{
  m_Target = pos;
  m_IsCalcViewMatrix = false;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! �J�����̏�����̐ݒ�
/*!
 	@param	vec	[i ]	�V�����ݒ肷����W
 */
void Camera::SetUpVector( const VECTOR3DF& vec )
{
  m_Up = vec;
  m_Up.Normalize();
  m_IsCalcViewMatrix = false;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! �r���[�}�g���N�X�̎擾
/*!
    @return �r���[�}�g���N�X
 */
const MATRIX4DF& Camera::GetViewMatrix() const
{
  if( !m_IsCalcViewMatrix )
  {
    Camera* pThis = const_cast<Camera*>(this);
    pThis->m_ViewMatrix.SetLookAt( m_Pos, m_Target, m_Up );

    pThis->m_IsCalcViewMatrix = true;
  }

  return m_ViewMatrix;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! �v���W�F�N�V�����}�g���N�X�̎擾
/*!
    @return �v���W�F�N�V�����}�g���N�X
 */
const MATRIX4DF& Camera::GetProjectionMatrix() const
{
  return m_ProjectionMatrix;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! �J�����̈ړ�
/*!
  �������W�����������Ɉړ����܂�

  @param  offset  [i ]  �ړ���
 */
void  Camera::Move( const VECTOR3DF& offset )
{
  Move( offset, offset );
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! �J�����̈ړ�
/*!
    @param  pos     [i ]  �J�����̍��W�̈ړ���
    @param  target  [i ]  �������W�̈ړ���
 */
void  Camera::Move( const VECTOR3DF& pos, const VECTOR3DF& target )
{
  m_Pos    += pos;
  m_Target += target;
  m_IsCalcViewMatrix = false;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! �J�������W�̎擾
/*!
    @return �J�������W
 */
const POINT3DF&  Camera::GetPosition() const
{
  return m_Pos;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! �������W�̎擾
/*!
    @return �������W
 */
const POINT3DF&  Camera::GetTarget  () const
{
  return m_Target;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! �J�����̏�����̎擾
/*!
    @return �J�����̏����
 */
const VECTOR3DF& Camera::GetUpVector() const
{
  return m_Up;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! �w�肵���R�c���W���J�������ɓ����Ă��邩���ׂ�
/*!
    @param  pos ���ׂ���W

    @return ����Ă���Ȃ� true
 */
bool Camera::IsCameraIn( const POINT3DF& pos ) const
{
  const MATRIX4DF& CameraMat = GetViewMatrix();
  const MATRIX4DF& ProjecMat = GetProjectionMatrix();

  MATRIX4DF vp = CameraMat * ProjecMat;

  const float x = pos.x*vp.m11 + pos.y*vp.m21 + pos.z*vp.m31 + vp.m41;
  const float y = pos.x*vp.m12 + pos.y*vp.m22 + pos.z*vp.m32 + vp.m42;
  const float z = pos.x*vp.m13 + pos.y*vp.m23 + pos.z*vp.m33 + vp.m43;

  POINT3DF p( x/z, y/z, ProjecMat.m33+ProjecMat.m43/z );

  return Collision<float>::IsPointToRect( p.x, p.y, p.z, -1, -1, -1, 1, 1, 1 );
}

}