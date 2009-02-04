#ifndef ioggdecoder_h
#define ioggdecoder_h


#include"../../source/config/define.h"

#include"oggpacket.h"
#include"ioggbuffer.h"

#include <boost/smart_ptr.hpp>


class IOggDecoder
{
public:
  virtual ~IOggDecoder(){}

  virtual void Initialize()=0;
  virtual void Finalize()=0;
  virtual void PacketIn( const OggPacket& NewPacket )=0;
  virtual double GetTime()=0;

  virtual void BeginSeekMode()=0;
  virtual void EndSeekMode()=0;

  //  初期設定が終わったか？
  virtual bool IsSetupped() const =0;

  //  デコードが始まっているか？（初期化ができただけではtrueにならない。さらにパケットをもらってOKになる）
  virtual bool IsDecoding() const =0;
  virtual SPOGGDECODEDBUFFER CreateDecodedBuffer() const =0;
};

typedef boost::shared_ptr<IOggDecoder> SPOGGDECODER;

#endif
