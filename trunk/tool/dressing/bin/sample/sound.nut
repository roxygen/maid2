
class SceneSample extends IScene
{
  _Sound = null;

  function Initialize()
  {
    IScene.Initialize();

    _Sound = CppSoundObject();
    _Sound.Type = PcmFile();
    _Sound.Type.FileName = "nc7802.ogg";
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

    //  �N���b�N����x�ɍĐ��E��~�̌J��Ԃ�
    if( GetInput().IsIn(MOUSE.L) )
    {
      _Sound.Play = !_Sound.Play;
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