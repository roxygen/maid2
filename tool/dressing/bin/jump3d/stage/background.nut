// ColorRect �̎g����

class BackGround extends DrawObject
{
  // squirrel�̌���d�l�ɂ�� �R���X�g���N�^���`����ꍇ��
  // �ꏏ�ɐe�̃R���X�g���N�^���ĂԂ���
  
  
  constructor()
  {
    DrawObject.constructor();
    
    Type = ColorRect(); //  �F�t�̎l�p�`�����܂�
    Type.Width = 800;   //  ����pixel
    Type.Height = 600;  //  �c��
    Type.ColorR = 0;    //  �F red
    Type.ColorG = 1;    //  �F green
    Type.ColorB = 1;    //  �F blue
    Type.Alpha = 1;     //  �s�����x (0.0�Ō����Ȃ��Ȃ�)

    Type.CenterX = 0;   // �I�t�Z�b�g�B��]�����鎞�ȂǂɎg����ʒu
    Type.CenterY = 0;

  }

}


