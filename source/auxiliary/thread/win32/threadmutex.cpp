﻿#include"threadmutex.h"


namespace Maid
{

	/*!
      @class	ThreadMutex threadmutex.h
      @brief	スレッド同期クラス
    \n			スレッド間の同期をとるのための mutex
    \n			プロセス間の同期用 mutex は ProcessMutex となります。
	*/

class ThreadMutex::Impl
{
public:
  Impl()  { ::InitializeCriticalSection( &m_Section ); }
  ~Impl() { ::DeleteCriticalSection( &m_Section ); }

  void Lock()   { ::EnterCriticalSection( &m_Section ); }
  void Unlock() { ::LeaveCriticalSection( &m_Section ); }

private:
  CRITICAL_SECTION m_Section;
};


ThreadMutex::ThreadMutex()
:m_pImpl( new Impl )
{
}

void ThreadMutex::Lock()
{
	m_pImpl->Lock();
}

void ThreadMutex::Unlock()
{
	m_pImpl->Unlock();
}

}

