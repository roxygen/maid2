/*
    �R�c�\���Ɏg�����C�g�N���X
    �P�V�[���P�ʂŎ����Ă��܂�
*/
class DirectionalLight
{
  Color = null
  Direction = null
  
  constructor()
  {
    Color = COLOR(1,1,1,1)
    Direction = VECTOR3D(1,0,0)
  }
}

class BaseLight
{
  Direction = null
  Ambient   = null

  constructor()
  {
    Direction = DirectionalLight()
    Ambient   = COLOR(0,0,0,0)
  
  }

}