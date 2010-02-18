import( "stage/scene.nut" );
import( "tittle/scene.nut" );

enum SCENEID
{
  TITTLE,
  STAGE,
}

class GameRoutine extends IGameRoutine
{
  static SCENEID_TITTLE = 0
  static SCENEID_STAGE  = 1

  _SceneID = 0

  // ��ԍŏ��̃V�[���쐬���ɌĂ΂�郁�\�b�h
  // ���̒��Ŏ��ɍs���V�[�����쐬����
  function Initialize()
  {
    _SceneID = SCENEID_TITTLE
    SetScene( SceneTittle() );
  }

  // ���ݎ��s���̃V�[�����I���������ƂɌĂ΂�郁�\�b�h
  // @param exitcode Scene.Finalize() �œn�����I�����
  function CreateNextScene( exitcode )
  {
    switch( _SceneID )
    {
    case SCENEID_TITTLE:
      {
        _SceneID = SCENEID_STAGE
        SetScene( SceneStage() );
      }break;
    
    case SCENEID_STAGE:
      {
        _SceneID = SCENEID_TITTLE
        SetScene( SceneTittle() );
      }break;
    }
  }
}

