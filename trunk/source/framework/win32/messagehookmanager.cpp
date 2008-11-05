#include"messagehookmanager.h"
#include"../../auxiliary/debug/assert.h"


namespace Maid
{

MessageHookManager* GlobalPointer<MessageHookManager>::s_pPointer;


MessageHookManager::MessageHookManager()
{
	m_pInterruptHook = NULL;
}

MessageHookManager::~MessageHookManager()
{
	Finalize();
}

void MessageHookManager::Initialize()
{
  Set();
}

void MessageHookManager::Finalize()
{
  Clear();
}

void MessageHookManager::AddHook( MessageHook* pHook, HWND hWnd )
{
  m_ProcMap[hWnd].push_front( pHook );
}

void MessageHookManager::DelHook( MessageHook* pHook, HWND hWnd )
{
  const HWNDMAP::iterator ite = m_ProcMap.find(hWnd);

  MAID_ASSERT(ite==m_ProcMap.end(), "�o�^����Ă��Ȃ��N���X���폜����悤�Ƃ��Ă��܂�" );

  HOOKLIST& List = ite->second;

  List.remove( pHook );
}

  //!	WM_CREATE�����E���Ƃ��p�̊��荞�݃t�b�N
  /*!	
	    @param pHook  [i ]  WM_CREATE�����ł���Ɗm�肵�Ă���|�C���^
  */
void MessageHookManager::SetInterruptHook( MessageHook* pHook )
{
  m_pInterruptHook = pHook;
}


LRESULT CALLBACK MessageHookManager::OnMessage( WindowsMessage& mess  )
{
  if( m_pInterruptHook!=NULL )
  {
    if( m_pInterruptHook->OnMessage( mess )==MessageHook::RETURNCODE_RETURN ) { return 0; }
  }else
  {
    HWNDMAP::iterator ProcList = m_ProcMap.find( mess.GetHWND() );
    if( ProcList!=m_ProcMap.end() )
    {
      HOOKLIST& List = ProcList->second;

      for( HOOKLIST::iterator ite=List.begin(); ite!=List.end(); ++ite )
      {	//	������ꂽ���b�Z�[�W������������ DefWindowProc �͌Ă΂Ȃ�
        if( (*ite)->OnMessage( mess )==MessageHook::RETURNCODE_RETURN ) { return 0; }
      }
    }  
  }

	return ::DefWindowProc(mess.GetHWND(),mess.GetMSG(),mess.GetWPARAM(),mess.GetLPARAM());
}

}