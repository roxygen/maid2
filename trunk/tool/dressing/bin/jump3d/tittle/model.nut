
class Model extends DrawObject
{
  _Count = 0
  
  constructor()
  {
    DrawObject.constructor();
    
    Type = ModelFile(); //  �F�t�̎l�p�`�����܂�
    Type.FileName = "block.mqo"
    Type.Alpha = 1;     //  �s�����x (0.0�Ō����Ȃ��Ȃ�)

    x = 300
    y = 400
  }

  function UpdateFrame()
  {
    _Count += 1;
    _Count %= 360
    
    Type.Matrix = MATRIX4D().SetRotationY( DEGtoRAD(_Count.tofloat()) )
  }


}


