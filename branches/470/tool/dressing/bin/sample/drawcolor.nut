// ColorRect �̎g����

class DrawObjectColor extends DrawObject
{
  // squirrel�̌���d�l�ɂ�� �R���X�g���N�^���`����ꍇ��
  // �ꏏ�ɐe�̃R���X�g���N�^���ĂԂ���
  
  
  constructor()
  {
    DrawObject.constructor();
    
    Type = ColorRect(); //  �F�t�̎l�p�`�����܂�
    Type.Width = 100;   //  ����pixel
    Type.Height = 200;  //  �c��
    Type.ColorR = 0;    //  �F red
    Type.ColorG = 0.5;  //  �F green
    Type.ColorB = 0;    //  �F blue
    Type.Alpha = 1;     //  �s�����x (0.0�Ō����Ȃ��Ȃ�)

    Type.CenterX = 0;   // �I�t�Z�b�g�B��]�����鎞�ȂǂɎg����ʒu
    Type.CenterY = 0;

  }

  function UpdateFrame()
  {
  }
}


class SceneSample extends IScene
{
  _obj = null

  function Initialize()
  {
    IScene.Initialize();

    _obj = DrawObjectColor();

    GetDrawObjectManager().InsertDrawObject( _obj );
  }

  function Finalize()
  {
    IScene.Finalize();
  }

  function IsExit()
  {
    return false;
  }

  function UpdateFrame()
  {
    IScene.UpdateFrame();
    
    local x = GetInput().GetPos(MOUSE.X);
    local y = GetInput().GetPos(MOUSE.Y);
    
    // �}�E�X���W�̈ʒu�ɕ\��������
    _obj.x = x;
    _obj.y = y;
    
    // L�{�^���������Ă�ԁA�΂��Z���Ȃ�
    if( GetInput().IsDown(MOUSE.L) )
    {
      _obj.Type.ColorG = 1.0;
    }else{
      _obj.Type.ColorG = 0.5;
    }
  }
}


class GameRoutine extends IGameRoutine
{
  // ��ԍŏ��̃V�[���쐬���ɌĂ΂�郁�\�b�h
  // ���̒��Ŏ��ɍs���V�[�����쐬����
  function CreateFirstScene()
  {
    return SceneSample();
  }

  // ���ݎ��s���̃V�[�����I���������ƂɌĂ΂�郁�\�b�h
  // \param exitcode Scene.GetExitCode() �œn�����I�����
  function CreateNextScene( exitcode )
  {
    SetScene( SceneSample() );
  }
}



//	���낢���`�����̂ŃQ�[���J�n�I
Run( GameRoutine() );

