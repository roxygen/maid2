//#define LOCAL_PROFILE

#include"oggdecoder.h"
#include"../auxiliary/debug/warning.h"
#include"../auxiliary/debug/profile.h"

namespace Maid
{

OggDecoder::OggDecoder( const IOggTimer& Timer )
  :m_State(STATE_EMPTY)
  ,m_IsNewPageIn(false)
  ,m_SleepCounter(0)
  ,m_Timer(Timer)
{

}

OggDecoder::~OggDecoder()
{
}

void OggDecoder::Initialize( const Xiph::SPOGGSTREAM& pStream, const Xiph::SPDECODER& pDecoder, const SPSAMPLECACHECHECKER& pChecker )
{
  Finalize();

  m_pStream  = pStream;
  m_pDecoder = pDecoder;
  m_pChecker = pChecker;

  m_State = STATE_DECOING;

  m_PacketTmp.reserve(30*1024);
  m_Thread.SetFunc( MakeThreadObject(&OggDecoder::ThreadFunction,this) );
  m_Thread.Execute();
}

void OggDecoder::Finalize()
{
  if( m_State==STATE_EMPTY ) { return ; }

  m_Thread.Close();

  m_pStream.reset();
  m_pDecoder.reset();
  m_pChecker.reset();
  m_Cache.Clear();
  m_IsNewPageIn = false;


  m_State = STATE_EMPTY;
}


void OggDecoder::PageIn( const Xiph::OggPage& page )
{

//  MAID_WARNING( "serial:" << page.GetSerialNo() << "page:" << page.GetPageNo() << "pack:"<< page.GetPacketCount());

  ThreadMutexLocker lock(m_StreamMutex);

  {
MAID_PROFILE();
    m_pStream->PageIn( page );
  }
  m_IsNewPageIn = true;
}


OggDecoder::STATE OggDecoder::GetState() const
{
  return m_State;
}

void OggDecoder::PopCache( double EndTime, Xiph::SAMPLELIST& Out )
{
MAID_PROFILE();

  m_Cache.Pop( EndTime, Out );
}


unt OggDecoder::ThreadFunction( volatile ThreadController::BRIGEDATA& state )
{
  while( true )
  {
    if( state.IsExit ) { break; }

    switch( m_State )
    {
    case STATE_EMPTY: { }break;
    case STATE_DECOING:   { OnStateDecoding();  }break;
    case STATE_PAGEWAIT:  { OnStatePageWait();  }break;
    case STATE_CACHEFULL: { OnStateCacheFull(); }break;
    case STATE_ENDOFSTREAM: { OnStateEndOfStream(); }break;
    }
  }

  m_pStream->Finalize();
  m_pDecoder->Finalize();
  return 0;
}

void OggDecoder::OnStateDecoding()
{
  if( m_pChecker->IsFull(m_Cache) )
  {
    m_State = STATE_CACHEFULL;
    return ;
  }

  m_SleepCounter = 0;

  Xiph::OggPacket packet;

  {
    ThreadMutexLocker lock(m_StreamMutex);
MAID_PROFILE();

    const bool IsSuccess = m_pStream->PacketOut(packet);

    if( !IsSuccess )
    {
      if( m_pStream->IsEnd() )
      {
        m_State = STATE_ENDOFSTREAM;
        return ;
      }else
      {
        m_IsNewPageIn = false;
        m_State = STATE_PAGEWAIT;
        return ;
      }
    }
    //  パケットのデータをスレッドで共有しないことによって
    //  同期してる時間を減らす
    packet.Copy( m_PacketTmp, packet );
  }


  Xiph::SPBUFFER pBuffer;
  const double begin = m_pDecoder->GetTime();
  {
MAID_PROFILE();

  m_pDecoder->Decode( packet, pBuffer );
  }

  if( pBuffer.get()!=NULL )
  {
    Xiph::SAMPLE  sample;
    sample.BeginTime = begin;
    sample.EndTime   = m_pDecoder->GetTime();
    sample.pBuffer   = pBuffer;

    m_Cache.PushBack( sample );
  }

  //  デコード時刻 < 現在時刻 のケースだと、デコードが遅れていることになるので
  //  適当にすっ飛ばす

  {
    const double dec = m_pDecoder->GetTime();
    const double now = m_Timer.Get();
    const double sa = now - dec;

    if( 0.0 < sa )
    {
      m_pDecoder->BeginSeekMode( now + sa * 3);
    }
  }
}

void OggDecoder::OnStatePageWait()
{
MAID_PROFILE();

  if( !m_IsNewPageIn )
  {
    Sleep();
  }else
  {
    m_State = STATE_DECOING;
  }
}

void OggDecoder::OnStateCacheFull()
{
MAID_PROFILE();

  if( m_pChecker->IsFull(m_Cache) )
  {
    Sleep();
  }else
  {
    m_State = STATE_DECOING;
  }
}

void OggDecoder::OnStateEndOfStream()
{
  Sleep();
}


void OggDecoder::Sleep()
{
  ++m_SleepCounter;

  if( m_SleepCounter%10 == 0 )
  {
    //  連続して寝るにつれて、時間が長くなる
    unt count = std::min((unt)5,m_SleepCounter/10);
    ThreadController::Sleep(count);
  }
}


}