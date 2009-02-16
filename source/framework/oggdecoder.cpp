//#define LOCAL_PROFILE

#include"oggdecoder.h"
#include"../auxiliary/debug/warning.h"
#include"../auxiliary/debug/profile.h"
#include"../auxiliary/debug/assert.h"

namespace Maid
{

OggDecoder::OggDecoder()
  :m_pTimer(NULL)
  ,m_TargetTime(0)
{

}

OggDecoder::~OggDecoder()
{
  Finalize();
}

void OggDecoder::Initialize( const IOggTimer& Timer, const Xiph::SPOGGSTREAM& pStream, const Xiph::SPCODEC& pCodec )
{
  Finalize();

  m_pTimer  = &Timer;
  m_pStream = pStream;
  m_pCodec  = pCodec;
}

void OggDecoder::Finalize()
{
  m_pStream.reset();
  m_pCodec.reset();
  m_pTimer = NULL;
  m_TargetTime = 0;
}


void OggDecoder::PageIn( const Xiph::OggPage& page )
{
  m_pStream->PageIn( page );
}



void OggDecoder::BeginSeek( double TargetTime )
{
  MAID_ASSERT( TargetTime < 0, "負数は指定できません" << TargetTime );

  m_TargetTime = TargetTime;
}

bool OggDecoder::IsSeeking() const
{
  return 0<m_TargetTime;
}

int  OggDecoder::GetPacketCount() const
{
  return m_pStream->GetPacketCount();
}

bool  OggDecoder::IsPacketEmpty() const
{
  return GetPacketCount()==0;
}


void OggDecoder::Update( OGGSAMPLE& out )
{
  if( IsStreamEnd() ) { return; }
  if( IsPacketEmpty() ) { return; }


  Xiph::OggPacket packet;
  const bool IsSuccess = m_pStream->PacketOut(packet);
  if( !IsSuccess ) { return ; }

  if( !m_pCodec->IsSetupped() )
  {
    m_pCodec->Setup(packet);
    return ;
  }


  const double OldTime = m_pCodec->GetTime();
  const double SampleLen = m_pCodec->CalcPacketLength(packet);

  if( IsSeeking() )
  {
    //  シーク中の場合は追いつくまでスキップスキップ　らんらん・・・・るーぅぅぅぅ！
    const double Target = m_TargetTime;

    if( OldTime+SampleLen < Target )
    {
      m_pCodec->Skip( packet );
      return ;
    }

    //  追いついた。
    m_TargetTime = 0;
  }

  {
    Xiph::SPBUFFER pBuffer;
    m_pCodec->Decode( packet, pBuffer );

    if( pBuffer.get()!=NULL )
    {
      out.BeginTime = OldTime;
      out.EndTime   = OldTime+SampleLen;
      out.pBuffer   = pBuffer;
    }
  }

  { //  もし遅れてたらスキップモード
    const double NowTime = m_pTimer->Get();

    if( OldTime+SampleLen < NowTime )
    {
      BeginSeek( NowTime );
    }
  }
}

}