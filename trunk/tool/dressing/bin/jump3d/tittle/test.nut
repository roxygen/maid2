// ColorRect �̎g����

class Test3D extends DrawObject
{
  // squirrel�̌���d�l�ɂ�� �R���X�g���N�^���`����ꍇ��
  // �ꏏ�ɐe�̃R���X�g���N�^���ĂԂ���
  
  
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

}


