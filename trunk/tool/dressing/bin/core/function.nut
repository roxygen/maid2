


function GetDirectryName( path )
{
  local pos = path.len()-1
  
  while( true )
  {
    if( pos==0 ) { break; }
    if( path[pos]=='\\' || path[pos]=='/' )
    {
      return path.slice(0,pos)
    }
    --pos
  }
  
  return ""
}

function GetFileName( path )
{
  local pos = path.len()-1
  
  while( true )
  {
    if( pos<=0 ) { return path; }
    if( path[pos]=='\\' || path[pos]=='/' )
    {
      return path.slice(pos+1)
    }
    --pos
  }
  
  return ""
}


//!   �V����squirrel�t�@�C����������
/*!
  @param  filename  [i ] ������t�@�C����

  @return �ǉ��ɐ��������� true
          ���s������false
 */
import_dir <- ""

function import( filename )
{
  //  filename���f�B���N�g�����Ă��肷���
  //  �\�[�X�����Ȃ񂩋C���������Ȃ�̂�
  //  �ꎞ�I�Ɋo���Ă���
  local dir  = GetDirectryName(filename)
  local name = GetFileName(filename)
  local olddir = import_dir

  import_dir += dir

  local path = ""
  if( import_dir.len()==0 ) { path = name }
  else { path = import_dir + "/" + name }

  Import( path )
  
  import_dir = olddir

  return FUNCTIONRESULT.SUCCESS;
}


gameloop <- true;

function ExitGameLoop()
{
  gameloop = false;
}

function Run( routine )
{
  storage <- Storage()
  storage.Load( "storage.xml" )

  admin <- SceneAdmin();// ���̕ϐ���C++��������A�N�Z�X���Ă�̂Ŗ��O��ς��Ȃ�����
  input <- KeyInput();

  admin.Initialize( routine, input );

  while( gameloop )
  {
    input.Update()
    admin.UpdateFrame();

    suspend(["nextframe"]);
  }

  admin.Finalize();
  storage.Save()
  delete input
  delete admin
  delete storage

  collectgarbage();
}

