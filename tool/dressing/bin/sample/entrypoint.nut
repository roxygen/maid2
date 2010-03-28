class DrawObjectColor extends DrawObject
{
  // squirrel�̌���d�l�ɂ�� �R���X�g���N�^���`����ꍇ��
  // �ꏏ�ɐe�̃R���X�g���N�^���ĂԂ���
  
  
  _Sound = null;
  
  constructor()
  {
    DrawObject.constructor();
    
    Type = ColorRect();
    Type.Width = 100;
    Type.Height = 200;
    Type.ColorG = 0.5;
    
    _Sound = CppSoundObject();
    _Sound.Type = PcmFile();
    _Sound.Type.FileName = "nc7802.ogg";
    
  }

  function Initialize()
  {
    _Sound.Play = true;
  }

  function UpdateFrame()
  {
  }
}


class DrawObjectImage extends DrawObject
{
  constructor()
  {
    DrawObject.constructor();
    Type = ColorText();
    Type.Text = "��������a��"
/*    
    Type = ImageRect();
    Type.RectX = 10;
    Type.RectY = 10;
    Type.RectW = 100;
    Type.RectH = 200;
    Type.FileName = "nc1673.bmp";
*/    
    x = 100;
  }

  function UpdateFrame()
  {
  }
}


class SceneSample extends IScene
{
  _obj_parent = null

  function Initialize()
  {
    IScene.Initialize();

    _obj_parent = DrawObjectColor();
    local obj_chiled = DrawObjectImage();
    _obj_parent.AddChiled( obj_chiled )

    GetDrawObjectManager().InsertDrawObject( _obj_parent );
    GetDrawObjectManager().InsertDrawObject( obj_chiled );
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
    
//    _obj_parent.x = x;
//    _obj_parent.y = y;
    
    if( GetInput().IsIn(MOUSE.L) )
    {
      _obj_parent.Kill();
    }else if( GetInput().IsIn(MOUSE.R) )
    {
      GetDrawObjectManager().InsertDrawObject( _obj_parent );
    }else
    {
      if( GetInput().IsDown('Z') )
      {
        _obj_parent.z += 1
      }else
      {
        _obj_parent.z = -1
      }

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

