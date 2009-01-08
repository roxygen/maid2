#include"soundobjectpcmstatic.h"


namespace Maid {

void SoundObjectPCMStatic::Initialize( const Sound::SPBUFFER& pBuffer  )
{
  m_pBuffer = pBuffer;
  m_IsLoop = false;
}

void SoundObjectPCMStatic::Update()
{

}

void SoundObjectPCMStatic::Play()
{
  m_pBuffer->Play(m_IsLoop);
}

void SoundObjectPCMStatic::Stop()
{
  m_pBuffer->Stop();
}

void SoundObjectPCMStatic::SetPosition( float time )
{
  const Sound::CREATEBUFFERPARAM& param = m_pBuffer->GetParam();

  const size_t pos = param.Format.CalcLength(time);

  m_pBuffer->SetPosition(pos);
}

void SoundObjectPCMStatic::SetVolume( float volume )
{
  m_pBuffer->SetVolume(volume);
}

void SoundObjectPCMStatic::SetJumpPoint( const std::vector<JUMPPOINT>& list )
{
  //  効果なし。
}

bool SoundObjectPCMStatic::IsPlay()const
{
  return m_pBuffer->IsPlay();
}

float SoundObjectPCMStatic::GetPosition() const
{
  const size_t p = m_pBuffer->GetPlayPosition();

  const Sound::CREATEBUFFERPARAM& param = m_pBuffer->GetParam();

  const float ret = param.Format.CalcTime(p);

  return ret;
}


void SoundObjectPCMStatic::SetLoopState( bool IsLoop )
{
  m_IsLoop = IsLoop;

  if( m_pBuffer->IsPlay() )
  {
    m_pBuffer->Play(IsLoop);
  }
}

}


