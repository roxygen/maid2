/*!
  @class  DrawObject drawobject.nut
  @brief  �`��I�u�W�F�N�g�N���X
  \n      �Q�[����ʂɉ����\�����������ꍇ�́A���̃N���X���p�����܂�
  \n      �\���^�C�v�� this.Type �����o�ɓ�����OK�ł�
*/
class DrawObject extends CppDrawObject
{
  _Manager = null
  _Chiled = null  //  �e�q�֌W�ɂ��� DrawObject 

  constructor()
  {
    CppDrawObject.constructor();
    _Chiled = []
  }


  // ���삪�J�n����鎞�ɌĂ΂��
  function Initialize()
  {
  }

  // ���삪�I������鎞(Kill()���Ă΂ꂽ��)�ɌĂ΂��
  function Finalize()
  {
    // �q�ɐe�̏��������Ă���
    foreach( ch in _Chiled )
    {
      local pos = ch.CalcGlobalPos();
      ch.x = pos.x
      ch.y = pos.y
      ch.z = pos.z
      ch.Parent = null
    }
    _Chiled = []
  }

  // �t���[�����ɌĂ΂��֐�
  // �e�폈���͂����ŏ�������
  function UpdateFrame()
  {
  }


  // ���̃I�u�W�F�N�g���p�ς݂ɂȂ�����Ă�
  // ���̃t���[������ UpdateFrame() ���Ă΂�Ȃ��Ȃ�܂�
  function Kill()
  {
    _Manager.RemoveDrawObject( this )
  }

  // ch �Ɛe�q�֌W������
  // this ���e�� ch ���q�ł�
  // �e�q�֌W�ɂȂ�ƍ��W�n���ς��܂�
  function AddChiled( ch )
  {
    ch.Parent = this;
    _Chiled.push( ch );
  }
  
  function DelChiled( ch )
  {
    foreach( i, obj in _Chiled )
    {
      if( obj==ch ) { _Chiled.remove(i); break; }
    }
    
    local pos = ch.CalcGlobalPos();
    ch.x = pos.x
    ch.y = pos.y
    ch.z = pos.z
    ch.Parent = null
  }
  
  function DelParent()
  {
    if( ch.Parent==null ) { return ; }

    ch.Parent.DelChiled(this);
  }
  
  function SetManager( m )
  {
    _Manager = m
  }

  //! ��΍��W�����߂�
  /*!
      @return ��΍��W���������e�[�u�� {x,y,z}
   */
  function CalcGlobalPos()
  {
    if( Parent==null ) { return {x=this.x,y=this.y,z=this.z} }
    
    local tmp = Parent.CalcGlobalPos()
    tmp.x += x
    tmp.y += y
    tmp.z += z
    
    return tmp
  }
}


class DrawObjectManager extends Task
{
  _ObjectList= null;  // CppDrawObject ���p�������z��
  _InsertList= null;  // ���t���[���Ŏ��s�����I�u�W�F�N�g CppDrawObject ���p�������z��
  _RemoveList= null;  // ���t���[���ō폜�����I�u�W�F�N�g CppDrawObject ���p�������z��

  constructor()
  {
    _ObjectList= [];
    _InsertList= [];
    _RemoveList= [];
  }
  
  function Initialize()
  {
    UpdateInsertObject();
  }
  function Finalize()
  {
    // _RemoveList �ɐ�ɓ����Ă�����̂������ׂ������H
    foreach( obj in _ObjectList)
    {
      obj.Finalize();
    }
    _ObjectList.clear();
    _InsertList.clear();
    _RemoveList.clear();
  }

  function UpdateFrame()
  {
    foreach( obj in _ObjectList)
    {
      obj.UpdateFrame();
    }

    // �s�v�ɂȂ����I�u�W�F�N�g�̍폜
    {
      foreach( obj in _RemoveList )
      {
        obj.Finalize();
        foreach( index, ele in _ObjectList )
        {
          if( ele==obj ) { _ObjectList.remove(index); break; }
        }
      }
      _RemoveList.clear();
    }

    // �V�����쐬���ꂽ�I�u�W�F�N�g�̐ݒ�
    UpdateInsertObject();

  }

  function UpdateInsertObject()
  {
    foreach( obj in _InsertList )
    {
      _ObjectList.push( obj );
      obj.SetManager( this );
      obj.Initialize();
    }
    _InsertList.clear();
  }


  function GetAllDrawObject()
  {
    return _ObjectList;
  }

  function InsertDrawObject( obj )
  {
    _InsertList.push( obj );  
  }

  function RemoveDrawObject( obj )
  {
    _RemoveList.push( obj );  
  }

}



