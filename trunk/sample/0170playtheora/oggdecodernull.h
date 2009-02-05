#ifndef oggdecodernull_h
#define oggdecodernull_h


#include"../../source/config/define.h"

#include"ioggdecoder.h"


class OggDecoderNULL : public IOggDecoder
{
public:
  OggDecoderNULL():m_Initialized(false),m_Time(0){}
  virtual void Initialize(){}
  virtual void Finalize(){}
  virtual void PacketIn( const OggPacket& NewPacket )
  {
    if( m_Initialized ) { m_Time+=100.0;}
    else { m_Initialized = true; }
  }
  virtual double GetTime(){return m_Time;}

  virtual void BeginSeekMode(){}
  virtual void EndSeekMode(){}

  virtual bool IsSetupped() const { return m_Initialized; }
  virtual bool IsDecoding() const { return false; }
  virtual SPOGGDECODEDBUFFER CreateDecodedBuffer()const{ return SPOGGDECODEDBUFFER(); }

private:
  bool   m_Initialized;
  double m_Time;

};

#endif
