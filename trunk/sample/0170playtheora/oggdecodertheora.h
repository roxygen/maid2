#ifndef oggdecodertheora_h
#define oggdecodertheora_h


#include"../../source/config/define.h"
#include"../../source/auxiliary/macro.h"
#include"../../source/graphics/surface.h"
#include"../../source/auxiliary/timer.h"

#include"ioggdecoder.h"

#include <theora/theora.h>

class OggDecoderTheora : public IOggDecoder
{
public:
  OggDecoderTheora();
  virtual void Initialize();
  virtual void Finalize();
  virtual void PacketIn( const OggPacket& NewPacket );
  virtual double GetTime();

  virtual void BeginSeekMode();
  virtual void EndSeekMode();

  virtual bool IsSetupped() const;
  virtual bool IsDecoding() const;
  virtual SPOGGDECODEDBUFFER CreateDecodedBuffer() const;

  const theora_info& GetInfo() const;

private:

  void SetPPLevel( int level );
  void OnSetup( const OggPacket& NewPacket );
  void OnDecode( const OggPacket& NewPacket );

private:
  theora_info      m_TheoraInfo;
  theora_comment   m_TheoraComment;
  theora_state     m_TheoraState;

  int m_MaxPostProcessLevel;
  int m_CurrentPostProcessLevel;

  enum STATE
  {
    STATE_EMPTY,  //  何もしてない
    STATE_INITIALIZING, //  初期化中
    STATE_BEGINDECODE,  //  デコード開始できます
    STATE_DECODING,     //  デコード中
  };

  STATE m_State;

};


#endif