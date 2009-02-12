#include"oggtimer.h"
#include"../auxiliary/debug/warning.h"

static const unt  TIMESTOP  = ~0;

namespace Maid
{
  OggTimer::OggTimer()
    :m_StartTime(TIMESTOP)
    ,m_Offset(0)
  {

  }

  void OggTimer::Start()
  {
    Start(0);
  }

  void OggTimer::Start( double time )
  {
    m_Time = time;
    m_Offset = unt(time * 1000.0 + 0.5);
    m_StartTime = m_Timer.Get();
  }

  void OggTimer::Stop()
  {
    m_StartTime = TIMESTOP;
  }

  void OggTimer::Resume()
  {
    Start(m_Time);
  }

  void OggTimer::Update()
  {
    if( m_StartTime==TIMESTOP ) { return ; }
    const unt time = m_Timer.Get() - m_StartTime + m_Offset;
    m_Time = double(time) / 1000.0;
  }

}