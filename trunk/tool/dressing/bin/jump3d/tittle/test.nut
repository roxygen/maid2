// ColorRect �̎g����

class Test3D extends DrawObject
{
  // squirrel�̌���d�l�ɂ�� �R���X�g���N�^���`����ꍇ��
  // �ꏏ�ɐe�̃R���X�g���N�^���ĂԂ���
  
  _Scale = 0
  
  constructor()
  {
    DrawObject.constructor();
    
    Type = ColorRect3D(); //  �F�t�̎l�p�`�����܂�
    Type.Width = 100;   //  ����pixel
    Type.Height = 100;  //  �c��
    Type.ColorR = 0;    //  �F red
    Type.ColorG = 1;    //  �F green
    Type.ColorB = 1;    //  �F blue
    Type.Alpha = 1;     //  �s�����x (0.0�Ō����Ȃ��Ȃ�)

    Type.CenterX = 0;   // �I�t�Z�b�g�B��]�����鎞�ȂǂɎg����ʒu
    Type.CenterY = 0;

    x = 400
    y = 150
  }

  function UpdateFrame()
  {
    _Scale += 1
    _Scale %= 5
    
    if( _Scale==0 ) { _Scale = 1 }

    Type.Matrix = MATRIX4D().SetScale( 1, _Scale, 1 )
//                * MATRIX4D().SetScale( _Scale, 1, 1 )
                  *MATRIX4D().SetRotationY( DEGtoRAD(45.0) )

//                * MATRIX4D().SetRotationY( DEGtoRAD(45.0) )
                
  
  }


}


