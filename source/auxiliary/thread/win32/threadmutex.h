/*!
  @file
  @brief	スレッド同期クラス
 */

#ifndef maid2_auxiliary_thread_threadmutex_h
#define maid2_auxiliary_thread_threadmutex_h

#include"../../../config/define.h"
#include"../../../config/Win32.h"

namespace Maid
{
  class ThreadMutex
  {
  public:
    ThreadMutex();
    ~ThreadMutex();

    void Lock();
    void Unlock();

  private:
    CRITICAL_SECTION	m_Section;
  };

  class ThreadMutexLocker
  {
  public:
    ThreadMutexLocker(ThreadMutex& cs) : m_rSection(cs) { m_rSection.Lock(); }
    ~ThreadMutexLocker() { m_rSection.Unlock(); }
  protected:
    ThreadMutex& m_rSection;
  };
}

#endif