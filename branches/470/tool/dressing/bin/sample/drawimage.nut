// �摜�\��

class DrawObjectImage extends DrawObject
{
  constructor()
  {
    DrawObject.constructor();

    Type = ImageRect();   //  �摜�t�@�C����\������N���X
    Type.FileName = "nc1673.bmp"; //  �\������t�@�C�����B�Ή��t�@�C���� bmp,png,jpeg
    Type.RectX = 10;  //  �\���͈͂w
    Type.RectY = 10;  //  �\���͈͂x
    Type.RectW = 100; //  �\���͈�width
    Type.RectH = 200; //  height
    Type.Alpha = 1.0  //  �����x
    
    //  �摜�t�@�C���͓ǂݍ��ݏ���������s����A�����ɂ͕\������܂���
    //  ���̖��͂��̂�����������\��(�p�����[�^��ǉ����铙)
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

    _obj = DrawObjectImage();

    GetDrawObjectManager().InsertDrawObject( _obj );
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
    
    _obj.x = x;
    _obj.y = y;

    // L�{�^���������Ă�ԁA�s�����ɂȂ�B
    if( GetInput().IsDown(MOUSE.L) )
    {
      _obj.Type.Alpha = 1.0;
    }else{
      _obj.Type.Alpha = 0.5;
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


/*
�񓯊��ǂݍ���
import


*/