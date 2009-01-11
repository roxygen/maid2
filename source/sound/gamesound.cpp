#include"gamesound.h"


namespace Maid { 

  std::set<GameSound*> GameSound::s_ObjectList;

GameSound::GameSound()
  :m_IsPlaying(false)
  ,m_LoopCount(0)
  ,m_IsFadeOuting(false)
  ,m_PrevPosition(0)
  ,m_Volume(1)
  ,m_Type(TYPE_GLOBAL)
{
  s_ObjectList.insert( this );
}

GameSound::~GameSound()
{
  s_ObjectList.erase( this );
}

void GameSound::Load( TYPE type, const String& filename )
{
  Destroy();

  m_Sound.LoadFile( filename );

  m_RealVolume.Set(1,1,0);
  m_Volume = 1;
  m_Type = type;
}

void GameSound::Destroy()
{
  m_IsPlaying = false;
  m_LoopCount = 0;
  m_IsFadeOuting = false;
  m_Sound.Destroy();
  m_PrevPosition = 0;
  m_Volume = 1;
}

bool GameSound::IsLoading()const
{
  return m_Sound.IsLoading();
}

void GameSound::Play()
{
  Play(1);
}

void GameSound::Play( int count )
{
  Play( count, 0 );
}

void GameSound::Play(  int count, float pos  )
{
  MAID_ASSERT( count==0, "0は指定できません" );

  if( count==1 )
  {
    m_Sound.SetLoopState(false);
  }else
  {
    m_Sound.SetLoopState(true);
  }

  SetVolume(m_Volume,0);
  m_PrevPosition = pos;
  m_Sound.SetPosition(pos);
  m_Sound.Play();

  if( count==LOOPPLAY ) { m_LoopCount = LOOPPLAY; }
  else { m_LoopCount = count-1; }
  m_IsPlaying = true;
}

void GameSound::Stop()
{
  m_IsPlaying = false;
  m_Sound.Stop();
}

void GameSound::Resume()
{
  if( m_IsPlaying )
  {
    m_Sound.Play();
  }
}

void GameSound::Pause()
{
  if( m_IsPlaying )
  { 
    m_Sound.Stop();
  }
}

void GameSound::SetVolume( float volume, float time )
{
  //  GlobalPointer<GameSoundManager> で時間を変更してもらう
  const int frame = GlobalPointer<GameSoundManager>::Get()->CalcFrame( time );
  const float scale = GetScaleVolume();

  const float v = volume*scale;

  m_RealVolume.Set( m_RealVolume, v, frame );
  m_Volume = volume;
}

void GameSound::FadeOut( float time )
{
  SetVolume( 0, time );
  m_IsFadeOuting = true;
}

bool GameSound::IsPlay() const
{
  return m_Sound.IsPlay();
}

float GameSound::GetPosition() const
{
  return m_Sound.GetPosition();
}

float GameSound::GetVolume() const
{
  return m_Volume;
}

void GameSound::UpdateFrame()
{
  if( m_Sound.IsEmpty() ) { return ; }
  if( m_Sound.IsLoading() ) { return ; }

  {
    if( m_RealVolume.IsEnd() )
    {
      if( m_IsFadeOuting )
      {
        m_Sound.Stop();
        m_IsFadeOuting = false;
      }
    }else
    {
      ++m_RealVolume;
    }

    if( m_Sound.GetVolume()!=m_RealVolume )
    {
      m_Sound.SetVolume( m_RealVolume );
    }
  }

  if( m_LoopCount!=LOOPPLAY && m_LoopCount!=0 )
  {
    //  ループ回数をチェックする
    const float pos = m_Sound.GetPosition();

    if( pos < m_PrevPosition )
    { //  前回の位置より今回の位置が手前にある＝＝ループした。

      --m_LoopCount;
      if( m_LoopCount==0 ) { m_Sound.SetLoopState(false); }
    }

    m_PrevPosition = pos;
  }
}

void GameSound::OnGlobalVolumeChange()
{
  SetVolume( m_Volume, 100 );
}

float GameSound::GetScaleVolume()const
{
  GameSoundManager::TYPE t;

  switch( m_Type )
  {
  case TYPE_GLOBAL: { t = GameSoundManager::TYPE_GLOBAL; }break;
  case TYPE_SE:     { t = GameSoundManager::TYPE_SE; }break;
  case TYPE_BGM:    { t = GameSoundManager::TYPE_BGM; }break;
  case TYPE_VOICE:  { t = GameSoundManager::TYPE_VOICE; }break;
  }

  const float scale = GlobalPointer<GameSoundManager>::Get()->CalcScale( t );

  return scale;
}

}

