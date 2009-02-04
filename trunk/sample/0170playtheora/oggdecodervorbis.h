#ifndef oggdecodervorbis_h
#define oggdecodervorbis_h


#include"../../source/config/define.h"
#include"../../source/auxiliary/macro.h"

#include"ioggdecoder.h"

#include <vorbis/codec.h>

class OggDecoderVorbis : public IOggDecoder
{
public:
  OggDecoderVorbis();

  virtual void Initialize();
  virtual void Finalize();
  virtual void PacketIn( const OggPacket& NewPacket );
  virtual double GetTime();

  virtual void BeginSeekMode();
  virtual void EndSeekMode();

  virtual bool IsSetupped() const;
  virtual bool IsDecoding() const;
  virtual SPOGGDECODEDBUFFER CreateDecodedBuffer() const;

  const vorbis_info& GetInfo() const;

private:
  void OnSetup( const OggPacket& NewPacket );
  void OnDecode( const OggPacket& NewPacket );

private:
  vorbis_info      m_VorbisInfo;
  vorbis_comment   m_VorbisComment;
  vorbis_dsp_state m_VorbisState;
  vorbis_block     m_VorbisBlock;

  enum STATE
  {
    STATE_EMPTY,  //  何もしてない
    STATE_INITIALIZING, //  初期化中
    STATE_BEGINDECODE,  //  デコード開始できます
    STATE_DECODING,     //  デコード中
  };

  STATE m_State;

  bool m_IsSeekMode;
};


#endif