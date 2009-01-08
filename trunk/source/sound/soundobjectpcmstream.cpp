#include"soundobjectpcmstream.h"
#include"../auxiliary/debug/assert.h"


namespace Maid {

 #pragma  COMPILERMSG( "TODO:バッファフォーマットはデコーダから取得するべき。" )

void SoundObjectPCMStream::Initialize( const Sound::SPBUFFER& pBuffer, const SPPCMDECODER& pDecoder, const boost::shared_ptr<std::vector<unt08> >& pData )
{
  m_pBuffer = pBuffer;
  m_pDecoder= pDecoder;
  m_pData   = pData;
  m_IsLoopPlay = false;
  m_NowPlayPosition = 0;
  m_PrevBufferPosition = 0;
  m_WrittedBufferPosition = 0;

  UpdateBuffer();
}

void SoundObjectPCMStream::Update()
{
  const size_t bufferpos = m_pBuffer->GetPlayPosition();
  const size_t decoderpos= m_pDecoder->GetPosition();
  const size_t decoderlen= m_pDecoder->GetLength();
  m_NowPlayPosition += CalcLength( m_PrevBufferPosition, bufferpos );
  m_PrevBufferPosition = bufferpos;

  //  再生した量がデコーダーより長かったら、全部再生されたことになる
  if( decoderlen <= m_NowPlayPosition )
  {
    if( m_IsLoopPlay )
    {
      m_NowPlayPosition -= decoderlen;
    }else
    {
      if( m_pBuffer->IsPlay() ) { m_pBuffer->Stop(); }
    }
  }
  else
  { //  そうでないならバッファの更新なんだけど、ある程度まとめてやる
    const size_t sa = decoderpos - m_NowPlayPosition;
    if( sa < m_pBuffer->GetParam().Length/3 )
    {
      UpdateBuffer(); 
    }
  }
}

void SoundObjectPCMStream::Play()
{
  if( m_pDecoder->GetLength() <= m_NowPlayPosition ) { return; }
  m_pBuffer->Play(true);
}

void SoundObjectPCMStream::Stop()
{
  m_pBuffer->Stop();
}

void SoundObjectPCMStream::SetPosition( float time )
{
  const Sound::CREATEBUFFERPARAM& param = m_pBuffer->GetParam();
  const size_t pos = param.Format.CalcLength(time);

  m_pDecoder->SetPosition( pos );
  m_WrittedBufferPosition = m_pBuffer->GetWritePosition();
  UpdateBuffer(); 
  m_pBuffer->SetPosition( m_WrittedBufferPosition );
}

void SoundObjectPCMStream::SetVolume( float volume )
{
  m_pBuffer->SetVolume(volume);
}

void SoundObjectPCMStream::SetJumpPoint( const std::vector<JUMPPOINT>& list )
{
  m_JumpList = list;
}

void SoundObjectPCMStream::SetLoopState( bool IsLoop )
{
  m_IsLoopPlay = IsLoop;
}

bool SoundObjectPCMStream::IsPlay()const
{
  return m_pBuffer->IsPlay();
}

float SoundObjectPCMStream::GetPosition() const
{
  const size_t pos = m_NowPlayPosition + CalcLength( m_PrevBufferPosition, m_pBuffer->GetPlayPosition() );

  return m_pBuffer->GetParam().Format.CalcTime(pos);
}

void SoundObjectPCMStream::UpdateBuffer()
{
  const size_t writepos  = m_WrittedBufferPosition;
  const size_t DecodeLen = m_pBuffer->GetParam().Length / 2;

  //  リングバッファにデータを流すのはしんどいので
  //  いったん作ってからやる
  std::vector<unt08> tmp(DecodeLen);

  {
    const size_t len = m_pDecoder->Read( &(tmp[0]), DecodeLen );
    if( len<DecodeLen )
    { //  デコードが末端までいったら
      //  ループ再生か否かで、バッファの接続を決める
      if( m_IsLoopPlay )  
      {
        m_pDecoder->SetPosition(0);
        m_pDecoder->Read( &(tmp[len]), DecodeLen-len );
      }
      else
      {
        ZERO( &(tmp[len]), DecodeLen-len );
      }
    }
  }

  //  そんでもってながしこむ～
  {
    Sound::IBuffer::LOCKDATA dat;
    m_pBuffer->Lock( writepos, DecodeLen, dat );
    memcpy( dat.pData1, &(tmp[0]), dat.Data1Length );
    if( dat.pData2!=NULL ) { memcpy( dat.pData2, &(tmp[dat.Data1Length]), dat.Data2Length ); }
    m_pBuffer->Unlock( dat );
  }

  m_WrittedBufferPosition += DecodeLen;
  m_WrittedBufferPosition %= m_pBuffer->GetParam().Length;
}

size_t SoundObjectPCMStream::CalcLength( size_t prev, size_t now )const
{
  size_t ret;
  
  if( prev<=now ) { ret = now-prev; }
  else { ret = m_pBuffer->GetParam().Length - prev; }

  return ret;
}

}


