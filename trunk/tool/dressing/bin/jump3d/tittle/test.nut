
class Model extends DrawObject
{
  constructor()
  {
    DrawObject.constructor();
    
    Type = ModelFile(); //  �F�t�̎l�p�`�����܂�
    Type.FileName = "block.mqo"
    Type.Alpha = 1;     //  �s�����x (0.0�Ō����Ȃ��Ȃ�)

    x = 400
    y = 150
  }

  function UpdateFrame()
  {
    Type.Matrix = MATRIX4D().SetRotationY( DEGtoRAD(45.0) )
  }


}


