/*
    �R�c�\���Ɏg���J�����N���X
    �P�V�[���P�ʂŎ����Ă��܂�
*/
class Camera
{
  Eye = null
  Target = null
  Up = null

  Fov = 0
  Aspect = 0
  Near = 0
  Far = 0
  
  ParallaxEye    = 0.0  // �����̒����@���̎��p
  ParallaxTarget = 0.0  // �����̒����@���̎��p

  constructor()
  {
    // �f�t�H���g�͂Q�c���ۂ����[�h
    Set2DPosition( 800, 600, 1.0, 1000.0 )
    
    ParallaxEye = 5.0
    ParallaxTarget = ParallaxEye // �Ƃ肠�������s�@��
  }
  
  //! �R�c�Ȃ񂾂��ǁA�Q�c���ۂ��\�������邽�߂̐ݒ�ɂ���
  /*!
      @param  width  [i ]  �X�N���[���̉���(int)
      @param  height [i ]  �X�N���[���̏c��(int)
      @param  near   [i ]  �\�������ʎ�O�̋���(float)
      @param  far    [i ]  �\�������ʉ��̋���(float)
  */  
  function Set2DPosition( width, height, near, far )
  {
    local fov= DEGtoRAD(60.0)
    local hw = width  / 2
    local hh = height / 2
    local dep= hh.tofloat() / tan(fov/2.0)

    Eye = POINT3D(hw.tofloat(), hh.tofloat(), -dep )
    Target = POINT3D(hw.tofloat(), hh.tofloat(), 0 )
    Up = VECTOR3D(0,1,0)

    Fov = fov
    Aspect = width.tofloat() / height.tofloat()
    Near = near
    Far  = far    
  }
}

