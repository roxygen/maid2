#include"decodervorbis.h"
#include"../../auxiliary/debug/assert.h"
#include"../../auxiliary/debug/warning.h"

namespace Maid { namespace Xiph {


SampleCacheVorbis::SampleCacheVorbis( const PCMFORMAT& fmt )
:m_Format(fmt)
{

}

void SampleCacheVorbis::Flush( double TargetTime, SPBUFFER& pOutput )
{
  ThreadMutexLocker lock(m_Mutex);

  //  デコード後のデータが複数にあることがあるので、全部つなげる

  typedef std::list<SPBUFFERPCM> BUFFERLIST;
  BUFFERLIST bufferlist;

  double BeginTime=0;
  {
    //  リストがないなら終了
    if( m_List.empty() ){ return ; }

    SAMPLELIST& InfoList = m_List;

    BeginTime = InfoList.begin()->BeginTime;

    for( SAMPLELIST::iterator ite =InfoList.begin();
                              ite!=InfoList.end();
                                   ++ite )
    {
      //  指定した時間より、バッファが古い場合は破棄
      SAMPLE& sample = *ite;
      if( sample.EndTime < TargetTime ) { continue; }

      bufferlist.push_back( 
          boost::static_pointer_cast<BufferPCM>(ite->pBuffer)
        );
    }

    InfoList.clear();
  }

  if( bufferlist.empty() ) { return ; }

  SPBUFFERPCM pRet( new BufferPCM );


  {
    //  time がバッファの BeginTime で始まってるわけないので
    //  そのギャップを入れる
    const PCMFORMAT& fmt = m_Format;
    const float sa = float(TargetTime - BeginTime);
    const size_t len = fmt.CalcLength(sa);

    if( 0<len )
    {
      SPBUFFERPCM pBuff( new BufferPCM );
      pBuff->WaveData.Initialize();
      pBuff->WaveData.Resize( len );

      ZERO( pBuff->WaveData.GetPointer(0), len );

      bufferlist.push_front(pBuff); //  ほほいと挿入
    }
  }
  { 
    //  続いてバッファの長さを計算する
    size_t len = 0;
    for( BUFFERLIST::iterator ite=bufferlist.begin();
                              ite!=bufferlist.end();
                                ++ite )
    {
      len += (*ite)->WaveData.GetSize();
    }

    pRet->WaveData.Initialize();
    pRet->WaveData.Resize( len );
  }


  {
    size_t pos = 0;
    for( BUFFERLIST::iterator ite=bufferlist.begin();
                              ite!=bufferlist.end();
                                ++ite )
    {
      void* pDst = pRet->WaveData.GetPointer(pos);
      const void* pSrc = (*ite)->WaveData.GetPointer(0);
      const size_t len = (*ite)->WaveData.GetSize();

      ::memcpy( pDst, pSrc, len );

      pos += len;
    }
  }

  pOutput = pRet;
}

bool SampleCacheVorbis::IsFull() const
{
  ThreadMutexLocker lock(const_cast<ThreadMutex&>(m_Mutex));

  if( m_List.empty() ) { return false; }

  const double begin = m_List.front().BeginTime;
  const double end = m_List.back().EndTime;
  const double len = end - begin;

  return 2.0f < len;
}















DecoderVorbis::DecoderVorbis()
:m_State(STATE_EMPTY)
,m_DecodeStartTime(0)
,m_DecodeSample(0)
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
  m_DecodeSample = 0;
}

bool DecoderVorbis::IsSetupped() const
{
  return m_State==STATE_DECODING;
}

bool DecoderVorbis::IsInitialized() const
{
  return m_State!=STATE_EMPTY;
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
  if( !IsSetupped() ) { return ; }

  m_DecodeStartTime = TargetTime;
  m_DecodeSample = 0;
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

  const ogg_int64_t pos = m_DecodeSample;

  const size_t tani = GetFormat().CalcBytePerLength();

  const double ret = double(pos) / double(tani);

  return ret;
}

SPBUFFERPCM DecoderVorbis::CreateDecodedBuffer()
{
  MAID_ASSERT( !IsSetupped(), "まだ準備できていません" );

  vorbis_dsp_state& state = m_VorbisState;

  float **pcm;
  const int length = vorbis_synthesis_pcmout(&state,&pcm);

  if( length <= 0 ) { return SPBUFFERPCM(); }

  SPBUFFERPCM pBuffer( new BufferPCM );

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
    vorbis_synthesis_blockin(&m_VorbisState,&m_VorbisBlock);

    const double oldtime = GetTime();
    SPBUFFERPCM pBuffer = CreateDecodedBuffer();
    const double newtime = GetTime();

  //  あまりにも古かったらダメ
    if( newtime < m_DecodeStartTime ) { return ; }

    pOut = pBuffer;
/*
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
*/
  }
}

/*
void DecoderVorbis::Flush( OGGDECODESAMPLELIST& List )
{
  {
    ThreadMutexLocker lock(m_Mutex);
    List.swap( m_BufferingList );
  }
}
*/


}}
