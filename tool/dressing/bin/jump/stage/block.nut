//  �n�ʃI�u�W�F�N�g�B���@������

class Block extends DrawObject
{
  _Speed = 0  //  �X�N���[���X�s�[�h
  
  constructor( w, h, speed )
  {
    DrawObject.constructor();
    
    Type = ColorRect(); //  �F�t�̎l�p�`�����܂�
    Type.Width = w;   //  ����pixel
    Type.Height = h;  //  �c��
    Type.ColorR = 1.0;    //  �F red
    Type.ColorG = 0.5;  //  �F green
    Type.ColorB = 0;    //  �F blue
    Type.Alpha = 1;     //  �s�����x (0.0�Ō����Ȃ��Ȃ�)

    Type.CenterX = 0;   // �I�t�Z�b�g�B��]�����鎞�ȂǂɎg����ʒu
    Type.CenterY = 0;
    
    _Speed = speed
  }

  function UpdateFrame()
  {
    x -= _Speed
  }
  
  function GetCollision()
  {
    return RECT(x-Type.CenterX, y-Type.CenterY, Type.Width, Type.Height )
  }
}


