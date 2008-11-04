#include"threadmutex.h"


namespace Maid
{

	/*!
      @class	ThreadMutex threadmutex.h
      @brief	スレッド同期クラス
    \n			スレッド間の同期をとるのための mutex
    \n			プロセス間の同期用 mutex は CProcessMutex となります。
	*/

ThreadMutex::ThreadMutex()
{ 
	::InitializeCriticalSection( &m_Section );
}

ThreadMutex::~ThreadMutex()
{
	::DeleteCriticalSection( &m_Section );
}

void ThreadMutex::Lock()
{
	::EnterCriticalSection( &m_Section );
}

void ThreadMutex::Unlock()
{
	::LeaveCriticalSection( &m_Section );
}

}

