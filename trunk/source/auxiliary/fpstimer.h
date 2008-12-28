/*!
 	@file
 	@brief	FPS ���Ǘ�����^�C�}�[
*/

#ifndef maid2_auxiliary_fpstimer_h
#define maid2_auxiliary_fpstimer_h


#include"../config/define.h"
#include"../config/typedef.h"
#include"timer.h"

namespace Maid
{
  class FPSTimer
  {
  public:
    FPSTimer();
    ~FPSTimer();

    void Initialize();

    void Reset();
    bool IsTimerOver() const;

    void Sleep();

    void SetFPS( unt fps );

  private:
    Timer	m_Timer;

    unt	m_BeginTime;    //!<	�P�t���[���ڂ̊J�n����
    unt	m_TimeOverCount;//!<	�\�莞����A���ŉ߂��Ă����
    unt	m_NowFrame;     //!<	���݂̃t���[����

    unt	m_FPS;          //!<	�ێ�����e�o�r
  };


}


#endif
