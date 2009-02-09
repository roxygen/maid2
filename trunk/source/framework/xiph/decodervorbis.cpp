#include"decodervorbis.h"
#include"../../auxiliary/debug/assert.h"
#include"../../auxiliary/debug/warning.h"

namespace Maid { namespace Xiph {





DecoderVorbis::DecoderVorbis()
:m_State(STATE_EMPTY)
,m_DecodeStartTime(0)
{
  ZERO( &m_VorbisInfo, sizeof(m_VorbisInfo) );
  ZERO( &m_VorbisComment, sizeof(m_VorbisComment) );
  ZERO( &m_VorbisState, sizeof(m_VorbisState) );
  ZERO( &m_VorbisBlock, sizeof(m_VorbisBlock) );
}

DecoderVorbis::~DecoderVorbis()
{
  Finalize();
}


void DecoderVorbis::Initialize( const OggPacket& FirstPacket )
{
  Finalize();

  vorbis_info_init( &m_VorbisInfo );
  vorbis_comment_init( &m_VorbisComment );
  m_State = STATE_INITIALIZING;

  OnSetup( FirstPacket );
}


void DecoderVorbis::Finalize()
{
  if( m_State==STATE_EMPTY ) { return ; }

  vorbis_block_clear(&m_VorbisBlock);
  vorbis_dsp_clear(&m_VorbisState);
  vorbis_comment_clear(&m_VorbisComment);
  vorbis_info_clear(&m_VorbisInfo);
  m_State = STATE_EMPTY;
  m_DecodeStartTime = 0;
}


const vorbis_info& DecoderVorbis::GetInfo() const
{
  MAID_ASSERT( m_State==STATE_EMPTY, "まだ初期化されていません" );
  return m_VorbisInfo;
}

PCMFORMAT DecoderVorbis::GetFormat()
{
  MAID_ASSERT( m_State==STATE_EMPTY, "まだ初期化されていません" );

  PCMFORMAT fmt;

  fmt.SamplesPerSecond = m_VorbisInfo.rate;
  fmt.BitPerSamples = sizeof(int16)*8;
  fmt.Channels = m_VorbisInfo.channels;

  return fmt;
}

void DecoderVorbis::BeginSeekMode( double TargetTime )
{
  m_DecodeStartTime = TargetTime;
}


void DecoderVorbis::Decode( const OggPacket& NewPacket, SPBUFFER& pOut )
{
  switch( m_State )
  {
  case STATE_EMPTY:       { }break;
  case STATE_INITIALIZING:{ OnSetup ( NewPacket ); }break;
  case STATE_DECODING:    { OnDecode( NewPacket, pOut ); }break;
  }
}



double DecoderVorbis::GetTime()
{
  if( m_State!=STATE_DECODING ) { return 0; }

  if( m_VorbisState.granulepos < 0 ) { return 0; }

  const double ret = vorbis_granule_time(&m_VorbisState, m_VorbisState.granulepos );

  return ret;
}

SPBUFFERPCM DecoderVorbis::CreateDecodedBuffer()
{
  vorbis_dsp_state& state = m_VorbisState;

  float **pcm;
  const int length = vorbis_synthesis_pcmout(&state,&pcm);

  if( length <= 0 ) { return SPBUFFERPCM(); }

  SPBUFFERPCM pBuffer( new BufferPCM );

  const int channels = m_VorbisInfo.channels;
  const size_t channellen = sizeof(float32) * length;

  pBuffer->Data.resize( channels );

  for( int i=0; i<channels; ++i )
  {
    BufferPCM::BUFFER& buf = pBuffer->Data[i];

    buf.resize(length);

    ::memcpy( &(buf[0]), pcm[i], channellen );
  }

  vorbis_synthesis_read(&state,length);

  return pBuffer;
}

void DecoderVorbis::OnSetup( const OggPacket& NewPacket )
{
  const ogg_int64_t no = NewPacket.GetPacketNo();
  ogg_packet& packet = (ogg_packet&)NewPacket.Get();

  const int ret = vorbis_synthesis_headerin(&m_VorbisInfo,&m_VorbisComment,&packet);
  if( ret<0 )  { MAID_WARNING( "vorbis_synthesis_headerin" ); return; }

  //  ヘッダパケットを全部処理したら初期化しておく
  if( no==2 )
  {
    vorbis_synthesis_init(&m_VorbisState,&m_VorbisInfo);
    vorbis_block_init(&m_VorbisState,&m_VorbisBlock);

    m_State = STATE_DECODING;
  }
}

void DecoderVorbis::OnDecode( const OggPacket& NewPacket, SPBUFFER& pOut )
{
  ogg_packet& packet = (ogg_packet&)NewPacket.Get();

  const int ret = vorbis_synthesis(&m_VorbisBlock,&packet);

  if( ret==0 )
  { //  成功した！
    const double oldtime = GetTime();
    const int ret = vorbis_synthesis_blockin(&m_VorbisState,&m_VorbisBlock);

    SPBUFFERPCM pBuffer = CreateDecodedBuffer();
    const double newtime = GetTime();

    //  あまりにも古かったらダメ
//    if( newtime < m_DecodeStartTime ) { return ; }

    pOut = pBuffer;
  }
}


}}
