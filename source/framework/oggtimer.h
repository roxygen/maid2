#ifndef maid2_framework_oggtimer_h
#define maid2_framework_oggtimer_h

#include"../config/define.h"
#include"../auxiliary/timer.h"

namespace Maid
{
  class IOggTimer
  {
  public:
    virtual ~IOggTimer(){}

    virtual double Get() const=0;
  };


  class OggTimer
    :public IOggTimer
  {
  public:
    OggTimer();

    void Start();
    void Start( double time );
    void Stop();
    void Resume();
    void SetOffset( double time );

    virtual double Get() const;

  private:
    unt     m_StartTime;
    double  m_Offset;
    Timer   m_Timer;
  };
}

#endif