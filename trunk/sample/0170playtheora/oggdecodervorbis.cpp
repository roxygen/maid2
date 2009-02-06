#include"oggdecodervorbis.h"
#include"../../source/auxiliary/debug/assert.h"
#include"../../source/auxiliary/debug/warning.h"


using namespace Maid;

OggDecoderVorbis::OggDecoderVorbis()
:m_State(STATE_EMPTY)
,m_DecodeStartTime(0)
{
  ZERO( &m_VorbisInfo, sizeof(m_VorbisInfo) );
  ZERO( &m_VorbisComment, sizeof(m_VorbisComment) );
  ZERO( &m_VorbisState, sizeof(m_VorbisState) );
  ZERO( &m_VorbisBlock, sizeof(m_VorbisBlock) );
}

OggDecoderVorbis::~OggDecoderVorbis()
{
  Finalize();
}


void OggDecoderVorbis::Initialize()
{
  vorbis_info_init( &m_VorbisInfo );
  vorbis_comment_init( &m_VorbisComment );
  m_State = STATE_INITIALIZING;
}


void OggDecoderVorbis::Finalize()
{
  if( m_State==STATE_EMPTY ) { return ; }

  vorbis_block_clear(&m_VorbisBlock);
  vorbis_dsp_clear(&m_VorbisState);
  vorbis_comment_clear(&m_VorbisComment);
  vorbis_info_clear(&m_VorbisInfo);
  m_State = STATE_EMPTY;
}

bool OggDecoderVorbis::IsSetupped() const
{
  return m_State==STATE_DECODING;
}


const vorbis_info& OggDecoderVorbis::GetInfo() const
{
  MAID_ASSERT( !IsSetupped(), "まだ初期化されていません" );
  return m_VorbisInfo;
}

PCMFORMAT OggDecoderVorbis::GetFormat()
{
  MAID_ASSERT( !IsSetupped(), "まだ初期化されていません" );

  PCMFORMAT fmt;

  fmt.SamplesPerSecond = m_VorbisInfo.rate;
  fmt.BitPerSamples = sizeof(int16)*8;
  fmt.Channels = m_VorbisInfo.channels;

  return fmt;
}

void OggDecoderVorbis::BeginSeekMode( double TargetTime )
{
  if( !IsSetupped() ) { return ; }
  ThreadMutexLocker lock(m_Mutex);
  m_BufferingList.clear();
  m_DecodeStartTime = TargetTime;
  m_DecodeSample = 0;
}

bool OggDecoderVorbis::IsBuffering() const
{
  if( m_BufferingList.empty() ) { return true; }

  double len;

  {
    ThreadMutexLocker lock(const_cast<ThreadMutex&>(m_Mutex));
    const double begin = m_BufferingList.front().BeginTime;
    const double end = m_BufferingList.back().EndTime;
    len = end - begin;
  }

  return len < 2.0f;
}

void OggDecoderVorbis::PacketIn( const OggPacket& NewPacket )
{
  switch( m_State )
  {
  case STATE_EMPTY:       { }break;
  case STATE_INITIALIZING:{ OnSetup ( NewPacket ); }break;
  case STATE_DECODING:    { OnDecode( NewPacket ); }break;
  }
}



double OggDecoderVorbis::GetTime()
{
  if( m_State!=STATE_DECODING ) { return 0; }

  const ogg_int64_t pos = m_DecodeSample;

  const size_t tani = GetFormat().CalcBytePerLength();

  const double ret = double(pos) / double(tani);

  return ret;
}

SPOGGDECODEDBUFFERPCM OggDecoderVorbis::CreateDecodedBuffer()
{
  MAID_ASSERT( !IsSetupped(), "まだ準備できていません" );


  vorbis_dsp_state& state = m_VorbisState;

  float **pcm;
  const int length = vorbis_synthesis_pcmout(&state,&pcm);

  if( length <= 0 ) { return SPOGGDECODEDBUFFERPCM(); }

  SPOGGDECODEDBUFFERPCM pBuffer( new OggDecodedBufferPCM );

  const int channels = m_VorbisInfo.channels;

  pBuffer->WaveData.Initialize();
  pBuffer->WaveData.Resize( length*channels*sizeof(int16) );

  int16* dst = (int16*)pBuffer->WaveData.GetPointer(0);

  int index = 0;
  for( int i=0; i<length; ++i )
  {
    for( int j=0; j<channels; ++j )
    {
      int val = int(pcm[j][i]*32767.0f + 0.5f);
      if(val> 32767){ val= 32767; }
      ef(val<-32768){ val=-32768; }
      dst[index] = val;
      ++index;
    }
  }

  vorbis_synthesis_read(&state,length);

  m_DecodeSample += pBuffer->WaveData.GetSize();

  return pBuffer;
}

void OggDecoderVorbis::OnSetup( const OggPacket& NewPacket )
{
  const ogg_int64_t no = NewPacket.GetPacketNo();
  ogg_packet& packet = (ogg_packet&)NewPacket.Get();

  const int ret = vorbis_synthesis_headerin(&m_VorbisInfo,&m_VorbisComment,&packet);
  if( ret<0 )  { MAID_WARNING( "theora_decode_header" ); return; }

  //  ヘッダパケットを全部処理したら初期化しておく
  if( no==2 )
  {
    vorbis_synthesis_init(&m_VorbisState,&m_VorbisInfo);
    vorbis_block_init(&m_VorbisState,&m_VorbisBlock);

    m_State = STATE_DECODING;
  }
}

void OggDecoderVorbis::OnDecode( const OggPacket& NewPacket )
{
  ogg_packet& packet = (ogg_packet&)NewPacket.Get();

  const int ret = vorbis_synthesis(&m_VorbisBlock,&packet);

  if( ret==0 )
  { //  成功した！
    vorbis_synthesis_blockin(&m_VorbisState,&m_VorbisBlock);

    const double oldtime = GetTime();
    SPOGGDECODEDBUFFERPCM pBuffer = CreateDecodedBuffer();
    const double newtime = GetTime();

  //  あまりにも古かったらダメ
    if( newtime < m_DecodeStartTime ) { return ; }

    if( pBuffer.get()!=NULL )
    {
      OGGDECODESAMPLE sample;

      sample.BeginTime = oldtime;
      sample.EndTime = newtime;
      sample.pBuffer = pBuffer;

      {
        ThreadMutexLocker lock(m_Mutex);
        m_BufferingList.push_back( sample );
      }
    }
  }
}

void OggDecoderVorbis::Flush( OGGDECODESAMPLELIST& List )
{
  {
    ThreadMutexLocker lock(m_Mutex);
    List.swap( m_BufferingList );
  }
}
