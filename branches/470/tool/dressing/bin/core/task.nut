/*!
  @class  Task task.nut
  @brief  �^�X�N�N���X
  \n      �e�폈���͂��̃N���X���p�����Ď�������
  \n      �^�X�N�Ƃ́A�Ȃɂ�����̂P�����ƍl����Ƃ����B
  \n      ���̃N���X�͕`��@�\���������Ă͂����Ȃ��BDrawObject���g������
*/
class Task
{
  _Group   = null;

  // �^�X�N�̓��삪�J�n����鎞�ɌĂ΂��
  function Initialize()
  {
  }
  
  // �^�X�N�̓��삪�I������鎞(Kill()���Ă΂ꂽ��)�ɌĂ΂��
  function Finalize()
  {
  
  }
  
  // �t���[�����ɌĂ΂��֐�
  // �e�폈���͂����ŏ�������
  function UpdateFrame()
  {
  }

  // ���̃I�u�W�F�N�g���p�ς݂ɂȂ������}
  function Kill()
  {
    _Group.GetManager().RemoveTask( GetPriority(), this );
  }
 
  function SetGroup( g )
  {
    _Group = g;
  }
  
  function GetPriority()
  {
    return _Group.GetPriority();
  }
}

/*!
  @class  TaskGroup dressinglib.nut
  @brief  ������Task���ꊇ���đ��삷��N���X
*/
class TaskGroup
{
  _Task = null;
  _Manager = null;
  _Priority = 0;

  function Initialize( manager, priority )
  {
    _Task = [];
    _Manager = manager;
    _Priority= priority;
  }

  function Finalize()
  {
    _Task = null;
    _Manager = null;
    _Priority= 0;
  }


  function UpdateFrame()
  {
    foreach( obj in _Task )
    {
      obj.UpdateFrame();
    }
  }

  function InsertTask( task )
  {
    task.SetGroup(this);
    _Task.push( task );
  }

  function RemoveTask( task )
  {
    foreach( index, ele in _Task )
    {
      if( ele==task ) { _Task.remove(index); break; }
    }
  }
  
  function GetManager()
  {
    return _Manager;
  }
  function GetPriority()
  {
    return _Priority;
  }
  
  function GetTask()
  {
    return _Task;
  }
}

/*!
  @class  TaskManager dressinglib.nut
  @brief  Task��D��x�ɉ����ĊǗ�����N���X
*/
class TaskManager
{
  _GroupList = null; // �^�X�N���X�g _GroupList[xx].UpdateFrame()�Ƃ����

  _InsertList= null; // _TaskList�ɒǉ�����郊�X�g�B���g��[priority,task]�̃e�[�u��
  _RemoveList= null; // _TaskList����r������郊�X�g�B���g��[priority,task]�̃e�[�u��

  // ������
  // �߂�l�Ȃ�
  function Initialize()
  {
    _GroupList = [];
    _InsertList= [];
    _RemoveList= [];
    for( local i=0; i<10; ++i )
    {
      local group = TaskGroup();
      group.Initialize(this, i);
      _GroupList.push( group );
    }
  }

  // �I��
  function Finalize()
  {
    foreach( group in _GroupList )
    {
      local tasklist = group.GetTask();
      foreach( task in tasklist )
      {
        task.Finalize();
      }
      group.Finalize();
    }

    _GroupList = null;
    _InsertList= null;
    _RemoveList= null;
  }

  // �t���[�����̍X�V
  // �߂�l����
  function UpdateFrame()
  {
   // �I�u�W�F�N�g�̍X�V
    {
      foreach( group in _GroupList )
      {
        group.UpdateFrame();
      }
    }

    // �s�v�ɂȂ����I�u�W�F�N�g�̍폜
    {
      foreach( dat in _RemoveList )
      {
        dat.task.Finalize();
        _GroupList[dat.priority].RemoveTask( dat.task );
      }
      _RemoveList.clear();
    }

    UpdateInsertTask()
  }

  // �V�����쐬���ꂽ�I�u�W�F�N�g�̐ݒ�
  function UpdateInsertTask()
  {
    foreach( dat in _InsertList )
    {
      _GroupList[dat.priority].InsertTask( dat.task );
      dat.task.Initialize();
    }
    _InsertList.clear();
  }

  function InsertTask( p, t )
  {
   local dat =
    {
      priority=p
      task    =t
    }

    _InsertList.push( dat );  
  }

  function RemoveTask( p, t )
  {
    local dat =
    {
      priority=p
      task    =t
    }

    _RemoveList.push( dat );  
  }
}


