#ifndef framework_xiph_decodernull_h
#define framework_xiph_decodernull_h


#include"../../config/define.h"

#include"idecoder.h"
#include"isamplecache.h"

namespace Maid { namespace Xiph {


  class SampleCacheNULL
    : public ISampleCache
  {
  public:
    void Flush( double TargetTime, SPBUFFER& Out ){}
    bool IsFull() const{ return true;}
  };

  class DecoderNULL
    : public IDecoder
  {
  public:
    DecoderNULL(){}
    virtual void Initialize( const OggPacket& FirstPacket ){}
    virtual void Finalize(){}

    virtual void Decode( const OggPacket& NewPacket, SPBUFFER& pOut ){}
    virtual bool IsSetupped() const { return true; }
    virtual bool IsInitialized() const { return true; }

    virtual double GetTime(){ return 0.0; }
    virtual void BeginSeekMode( double TargetTime ){}
  };

}}

#endif
