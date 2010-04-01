//  �n�ʃI�u�W�F�N�g�B���@������

class Block extends DrawObject
{
  _Speed = 0  //  �X�N���[���X�s�[�h
  
  _Width = 0
  _Height = 0
  
  constructor( w, h, speed )
  {
    DrawObject.constructor();
    
    Type = ModelFile(); //  �F�t�̎l�p�`�����܂�
    Type.FileName = "block.mqo"
    Type.Alpha = 1;     //  �s�����x (0.0�Ō����Ȃ��Ȃ�)
    
    _Speed = speed
    
    _Width  = w
    _Height = h
    
    Type.Matrix = MATRIX4D().SetScale( w.tofloat()/200.0, h.tofloat()/200.0, 1.0 )
  }

  function UpdateFrame()
  {
    x -= _Speed
  }
  
  function GetCollision()
  {
    return RECT2D(x, y-_Height, _Width, _Height )
  }
}


