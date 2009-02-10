
#include"oggdecoder.h"
#include"../auxiliary/debug/warning.h"

namespace Maid
{

OggDecoder::OggDecoder()
  :m_State(STATE_EMPTY)
  ,m_IsNewPageIn(false)
{

}

void OggDecoder::Initialize( const Xiph::SPOGGSTREAM& pStream, const Xiph::SPDECODER& pDecoder, const SPSAMPLECACHECHECKER& pChecker )
{
  m_pStream  = pStream;
  m_pDecoder = pDecoder;
  m_pChecker = pChecker;

  m_State = STATE_DECOING;

  m_Thread.SetFunc( MakeThreadObject(&OggDecoder::ThreadFunction,this) );
  m_Thread.Execute();
}

void OggDecoder::PageIn( const Xiph::OggPage& page )
{
  ThreadMutexLocker lock(m_StreamMutex);

  m_pStream->PageIn( page );
  m_IsNewPageIn = true;
}


OggDecoder::STATE OggDecoder::GetState() const
{
  return m_State;
}

void OggDecoder::PopCache( double EndTime, Xiph::SAMPLELIST& Out )
{
  m_Cache.Pop( EndTime, Out );

/*
  if( 25497==m_pStream->GetSerialNo() )
  {
    MAID_WARNING( "Pop time:" << m_Cache.GetBeginTime() << "-" << m_Cache.GetBeginTime()+m_Cache.GetTotalTime() << " count:" << m_Cache.GetSize() );
  }
*/
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

  return 0;
}

void OggDecoder::OnStateDecoding()
{
  if( m_pChecker->IsFull(m_Cache) )
  {
    m_State = STATE_CACHEFULL;
    return ;
  }

  Xiph::OggPacket packet;

//  {
    ThreadMutexLocker lock(m_StreamMutex);

    const bool IsSuccess = m_pStream->PacketOut(packet);

    if( !IsSuccess )
    {
      m_IsNewPageIn = false;
      m_State = STATE_PAGEWAIT;
      return ;
    }
  // パケットのメモリエリアがストリームと共有されていなければ、
  // ここでロックをはずしたほうがパフォーマンスがいいはず
//  }

  Xiph::SPBUFFER pBuffer;

  const double begin = m_pDecoder->GetTime();
  m_pDecoder->Decode( packet, pBuffer );

  if( pBuffer.get()!=NULL )
  {
    Xiph::SAMPLE  sample;
    sample.BeginTime = begin;
    sample.EndTime   = m_pDecoder->GetTime();
    sample.pBuffer   = pBuffer;

    m_Cache.PushBack( sample );
  }
/*
  if( 25497==m_pStream->GetSerialNo() )
  {
    MAID_WARNING( "Push time:" << m_Cache.GetBeginTime() << "-" << m_Cache.GetBeginTime()+m_Cache.GetTotalTime() << " count:" << m_Cache.GetSize() );
  }
*/
  if( m_pStream->IsEnd() )
  {
    m_State = STATE_ENDOFSTREAM;
  }
}

void OggDecoder::OnStatePageWait()
{
  if( !m_IsNewPageIn )
  {
//    ThreadController::Sleep(1);
  }else
  {
    m_State = STATE_DECOING;
  }
}

void OggDecoder::OnStateCacheFull()
{
  if( m_pChecker->IsFull(m_Cache) )
  {
    ThreadController::Sleep(1);
  }else
  {
    m_State = STATE_DECOING;
  }
}

void OggDecoder::OnStateEndOfStream()
{
  ThreadController::Sleep(1);
}

}