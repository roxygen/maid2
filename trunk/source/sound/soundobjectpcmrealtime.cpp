#include"soundobjectpcmrealtime.h"
#include"../auxiliary/debug/assert.h"
#include"../auxiliary/debug/warning.h"
#include"../auxiliary/mathematics.h"


namespace Maid {

  /*!
      @class  SoundObjectPCMStream soundobjectpcmrealtime.h
      @brief  ストリーム書き込みで再生するクラス
  */

SoundObjectPCMRealTime::SoundObjectPCMRealTime()
  :ISoundObject(TYPE_REALTIME)
{

}

void SoundObjectPCMRealTime::Initialize(  const Sound::SPBUFFER& pBuffer, const SPPCMBUFFERLIST& pDecoder )
{
  m_pBuffer = pBuffer;
  m_pDecoder= pDecoder;
 
  m_TotalPlayPosition = 0;
  m_PrevBufferPosition = 0;
  m_WrittedBufferPosition = 0;
  UpdateBuffer();
}

void SoundObjectPCMRealTime::Update()
{
  const size_t bufferpos = m_pBuffer->GetPlayPosition();
  const size_t delta_len = CalcLength( m_PrevBufferPosition, bufferpos );

  m_TotalPlayPosition += delta_len;
  m_PrevBufferPosition = bufferpos;

  UpdateBuffer(); 
}

void SoundObjectPCMRealTime::Play()
{
  if( IsPlay() ){ return ; }

  UpdateBuffer();
  m_pBuffer->Play(true);
}

void SoundObjectPCMRealTime::Stop()
{
  m_pBuffer->Stop();
}

void SoundObjectPCMRealTime::SetPosition( double time )
{
  //  常に進んでるのでスルー

}

void SoundObjectPCMRealTime::SetVolume( double volume )
{
  //  強さから デシベルに変換する
  // http://e-words.jp/w/E38387E382B7E38399E383AB.html

  const double db = 20.0 * Math<double>::log10(volume);

  m_pBuffer->SetVolume(db);
}

void SoundObjectPCMRealTime::SetLoopState( bool IsLoop )
{
  //  常に進んでるのでスルー
}

bool SoundObjectPCMRealTime::IsPlay()const
{
  return m_pBuffer->IsPlay();
}

double SoundObjectPCMRealTime::GetPosition() const
{
  const size_t pos = m_TotalPlayPosition + CalcLength( m_PrevBufferPosition, m_pBuffer->GetPlayPosition() );

  return m_pBuffer->GetParam().Format.CalcTime(pos);
}

double SoundObjectPCMRealTime::GetVolume() const
{
  //  デシベルから 強さに変換する
  // http://e-words.jp/w/E38387E382B7E38399E383AB.html

  const double db = m_pBuffer->GetVolume();

  if( db < -100.0f ) { return 0; }

  //  db = 20.0f * Math<float>::log10(ret);
  //  db/20.0f = Math<float>::log10(ret);
  // 10 の db/20.0f 乗が ret 

  return Math<double>::pow(10,db/20.0f);
}

void SoundObjectPCMRealTime::UpdateBuffer()
{
  const size_t decoderlen = m_pDecoder->GetLength();

  if( decoderlen==0 ) { return ; }  //  データがないときはスルー

  const size_t writepos  = m_WrittedBufferPosition;
  const size_t updatesize = CalcUpdateScape();

  { //  バッファの更新は、ある程度まとめてやる
    const size_t sa    = m_pDecoder->GetPosition() - m_TotalPlayPosition;
    if( updatesize < sa ) { return ; }
  }


  //  リングバッファにデータを流すのはしんどいので
  //  いったん作ってからやる
  std::vector<unt08> tmp(updatesize);

  {
    const size_t len = m_pDecoder->Read( &(tmp[0]), updatesize );
    if( len<updatesize )
    {
      //  足りてなかったら無音を流すようにする
      //  更新場所を手前にするのもありかな？（追い抜かれたら大変だけど）
      ZERO( &(tmp[len]), updatesize-len );
    }
  }

  //  そんでもってながしこむ～
  {
    Sound::IBuffer::LOCKDATA dat;
    m_pBuffer->Lock( writepos, updatesize, dat );
    memcpy( dat.pData1, &(tmp[0]), dat.Data1Length );
    if( dat.pData2!=NULL ) { memcpy( dat.pData2, &(tmp[dat.Data1Length]), dat.Data2Length ); }
    m_pBuffer->Unlock( dat );
  }

  m_WrittedBufferPosition += updatesize;
  m_WrittedBufferPosition %= m_pBuffer->GetParam().Length;
}

size_t SoundObjectPCMRealTime::CalcLength( size_t prev, size_t now )const
{
  size_t ret;
  
  if( prev<=now ) { ret = now-prev; }
  else { ret = m_pBuffer->GetParam().Length - prev; }

  return ret;
}

size_t SoundObjectPCMRealTime::CalcUpdateScape()const
{
  //  更新間隔を求める

  MAID_ASSERT( m_pBuffer.get()==NULL, "初期化されていません" );
  return m_pBuffer->GetParam().Length / 4;
}

}


