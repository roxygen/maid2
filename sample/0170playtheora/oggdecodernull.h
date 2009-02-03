#ifndef oggdecodernull_h
#define oggdecodernull_h


#include"../../source/config/define.h"

#include"ioggdecoder.h"


class OggDecoderNULL : public IOggDecoder
{
public:
  virtual void Initialize(){}
  virtual void PacketIn( const OggPacket& NewPacket ){}
  virtual double GetTime(){return 0;}

  virtual void BeginSeekMode(){}
  virtual void EndSeekMode(){}

  virtual bool IsDecoding() const { return false; }
  virtual SPOGGDECODEDBUFFER CreateDecodedBuffer()const{ return SPOGGDECODEDBUFFER(); }
  virtual void Finalize(){}
};

#endif
