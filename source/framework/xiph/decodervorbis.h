#ifndef framework_xiph_decodervorbis_h
#define framework_xiph_decodervorbis_h


#include"../../config/define.h"
#include"../../auxiliary/macro.h"
#include"../../auxiliary/thread.h"
#include"../../sound/common.h"

#include"idecoder.h"

#include <vorbis/codec.h>

namespace Maid { namespace Xiph {

  class BufferPCM
    : public IBuffer
  {
  public:
    typedef std::vector<float32>  BUFFER;
    std::vector<BUFFER> Data; //  １チャンネルごとに並んでる
  };

  typedef boost::shared_ptr<BufferPCM> SPBUFFERPCM;



  class DecoderVorbis 
    : public IDecoder
  {
  public:
    DecoderVorbis();
    ~DecoderVorbis();

    virtual void Initialize( const OggPacket& FirstPacket );
    virtual void Finalize();

    virtual void Decode( const OggPacket& NewPacket, SPBUFFER& pOut );

    virtual double GetTime();

    virtual void BeginSeekMode( double TargetTime );

    const vorbis_info& GetInfo() const;

    PCMFORMAT GetFormat();

    virtual std::string GetDebugString( const OggPacket& NewPacket )const;

  private:
    void OnSetup( const OggPacket& NewPacket );
    void OnDecode( const OggPacket& NewPacket, SPBUFFER& pOut );
    SPBUFFERPCM CreateDecodedBuffer();

  private:
    vorbis_info      m_VorbisInfo;
    vorbis_comment   m_VorbisComment;
    vorbis_dsp_state m_VorbisState;
    vorbis_block     m_VorbisBlock;

    enum STATE
    {
      STATE_EMPTY,  //  何もしてない
      STATE_INITIALIZING, //  初期化中
      STATE_DECODING,     //  デコード中
    };

    STATE m_State;

    double  m_DecodeStartTime;  //  デコードを開始する時間
  };

}}

#endif