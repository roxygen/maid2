﻿#include"mediatimer.h"
#include"../../auxiliary/debug/warning.h"

static const unt  TIMESTOP  = ~0;

namespace Maid
{
  MediaTimer::MediaTimer()
    :m_StartTime(TIMESTOP)
    ,m_Offset(0)
  {

  }

  void MediaTimer::Start()
  {
    m_StartTime = m_Timer.Get();
  }

  void MediaTimer::Stop()
  {
    if( m_StartTime==TIMESTOP ) { return ; }
    const double now = Get();
    m_StartTime = TIMESTOP;
    m_Offset = now; //  再開用にとっておく
  }

  double MediaTimer::Get() const
  {
    if( m_StartTime==TIMESTOP ) { return m_Offset; }
    const double time = double(m_Timer.Get() - m_StartTime) / 1000.0;
    return time + m_Offset;
  }

  void MediaTimer::SetOffset( double time )
  {
    m_Offset = time;
  }

}