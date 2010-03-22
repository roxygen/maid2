class IScene
{
  _TaskManager       = null;
  _DrawObjectManager = null;
  _Admin = null;  //  ���̃V�[�����Ǘ����Ă� SceneAdmin

  _Camera = null

  constructor()
  {
  }


  // ���̃V�[���̏�����
  // �߂�l�Ȃ�
  function Initialize()
  {
    _Camera = Camera()
    _TaskManager = TaskManager();
    _DrawObjectManager = DrawObjectManager();
    _TaskManager.Initialize();
    _TaskManager.InsertTask( 5, _DrawObjectManager );
  }

  // ���̃V�[���̊J��
  // ���̊֐����Ăяo������A�������s�ł��܂���
  // ���̃V�[���̏I����ԁi�e�����߂邱�Ɓj
  function Finalize()
  {
    _TaskManager.Finalize();
    _TaskManager = null;
    _DrawObjectManager = null;
  }

  // ���̃V�[�����I�����Ă��������H
  // �I��OK�Ȃ� true , NG�Ȃ�false
  function IsExit()
  {
    return false;
  }

  // �t���[�����̍X�V
  // �߂�l����
  function UpdateFrame()
  {
    _TaskManager.UpdateFrame();
  }

  //	�`��Ɏg���Q�[���I�u�W�F�N�g���擾����
  function GetAllDrawObject()
  {
    return _DrawObjectManager.GetAllDrawObject();
  }
  
  function GetDrawObjectManager()
  {
    return _DrawObjectManager;  
  }
  
  function GetInput()
  {
    return _Admin._KeyInput;
  }
}

class SceneAdmin
{
  _GameRoutine = null
  _SceneStack  = null // �V�[���z��B������[0]���ŏ��B�`���[0]����ԉ�
  _KeyInput    = null // �L�[�{�[�h�E�}�E�X�̏��

  function Initialize( routine, input )
  {
    _GameRoutine = routine
    _GameRoutine._SceneAdmin = this;
    _SceneStack= [null]
    _KeyInput = input;
    
    _GameRoutine.Initialize();
  }

  function Finalize()
  { // CppDrawObject �������Ă��� hDrawType ��cpp������ sq_release() ���邽�߂�
    // �t�@�C�i���C�U�̌Ăяo�����K�{�ɂȂ�B
    {
      // �쐬���x�����ɏI����Ă���
      while( true )
      {
        if( _SceneStack.len()==0 ) { break; }
        _SceneStack.top().Finalize();
        _SceneStack.pop();
      }
    }

    _GameRoutine = null;
    _SceneStack= null;
    _KeyInput = null;
  }

  function UpdateFrame()
  {
    if( _SceneStack.top().IsExit() )
    {
      suspend(["scenefade",100]);
      local retcode = _SceneStack.top().Finalize();
      _GameRoutine.CreateNextScene( retcode );
    }else
    {
      _SceneStack.top().UpdateFrame();
    }
  }

  function GetDrawObject()
  {
    local ret = [];
    foreach( scene in _SceneStack )
    {
      local tmp = 
      {
        camera = scene._Camera,
        object = scene.GetAllDrawObject()
      }
      ret.push( tmp );
    }
    return ret;
  }

  function SetScene( scene )
  {
    scene._Admin = this;
    scene.Initialize()
    _SceneStack[_SceneStack.len()-1] = scene;
  }

  function PushScene( scene )
  {
    scene._Admin = this;
    scene.Initialize()
    _SceneStack.push( scene );
  }

  function PopScene()
  {
    _SceneStack.pop()
  }
}

