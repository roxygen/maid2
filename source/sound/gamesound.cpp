﻿#include"gamesound.h"

#include"../auxiliary/debug/warning.h"

namespace Maid { 

  /*!
      @class  GameSound gamesound.h
      @brief  ゲームで使うような処理を持っているサウンドクラス
  \n          このクラスは GameSoundManager と連携しているので注意
  */


  std::set<GameSound*> GameSound::s_ObjectList;

GameSound::GameSound()
  :m_PlayCount(0)
  ,m_IsPlaying(false)
  ,m_IsFadeOuting(false)
  ,m_PrevPosition(0)
  ,m_Volume(1)
  ,m_Type(TYPE_GLOBAL)
{
  s_ObjectList.insert( this );
}


GameSound::GameSound( const GameSound& rhs )
  :m_Sound(rhs.m_Sound)
  ,m_PlayCount(rhs.m_PlayCount)
  ,m_IsPlaying(rhs.m_IsPlaying)
  ,m_RealVolume(rhs.m_RealVolume)
  ,m_IsFadeOuting(rhs.m_IsFadeOuting)
  ,m_PrevPosition(rhs.m_PrevPosition)
  ,m_Volume(rhs.m_Volume)
  ,m_Type(rhs.m_Type)
{
  s_ObjectList.insert( this );
}


GameSound::~GameSound()
{
  s_ObjectList.erase( this );
}

//! サウンドファイルを読み込む
/*!
    読み込みは別スレッドで行われます

 	  @param	type      [i ]	使用用途
 	  @param	filename	[i ]	ファイル名
 */
void GameSound::Load( TYPE type, const String& filename )
{
  Destroy();

  m_Sound.LoadFile( filename );

  m_Type = type;
}

void GameSound::Load( TYPE type, const String& filename, const SOUNDJUMPPOINTLIST& List )
{
  Destroy();

  m_Sound.LoadFileStream( filename, List );

  m_Type = type;

}


//! 読み込んでいたファイルを破棄する
/*!
 */
void GameSound::Destroy()
{
  m_IsPlaying = false;
  m_PlayCount = 0;
  m_IsFadeOuting = false;
  m_Sound.Destroy();
  m_PrevPosition = 0;
  m_RealVolume.Set(1,1,0);
  m_Volume = 1;

}

//! 読み込み中か？
/*!
 */
bool GameSound::IsLoading()const
{
  return m_Sound.IsLoading();
}



//! 先頭から１回だけ鳴らす
/*!
 */
void GameSound::Play()
{
  Play(1);
}


//! 先頭から複数回鳴らす
/*!
    @param  count [i ]  鳴らす回数
 */
void GameSound::Play( int count )
{
  SetPlayCount( count );
  _Play();
}

//! 任意の位置から複数回鳴らす
/*!
    ２週目は time=0 からの再生になります

    @param  count [i ]  鳴らす回数
    @param  pos   [i ]  再生開始時間
 */
void GameSound::Play(  int count, double pos  )
{
  SetPlayCount( count );
  SetPosition(pos);
  _Play();
}

//! 再生をとめる
/*!
 */
void GameSound::Stop()
{
  m_IsPlaying = false;
  m_PlayCount = 0;
  m_Sound.Stop();
}

//! 一時停止から再生する
/*!
 */
void GameSound::Resume()
{
  if( m_IsPlaying )
  {
    m_Sound.Play();
  }
}

//! 再生を一時停止します
/*!
    とめたところから再生させる場合は Resume() を呼ぶこと
 */
void GameSound::Pause()
{
  if( m_IsPlaying )
  { 
    m_Sound.Stop();
  }
}


//! ボリュームの設定
/*!
    原理上 2.0f とかできるけど、まだ実装してない。

    @param  volume  [i ]  新しいボリューム(0.0f:無音 1.0f:等倍)
 */
void GameSound::SetVolume( double volume, double time )
{
  //  GlobalPointer<GameSoundManager> で時間を変更してもらう
  const int frame = GlobalPointer<GameSoundManager>::Get()->CalcFrame( time );
  const double scale = GetScaleVolume();

  const double v = volume*scale;

  m_RealVolume.Set( m_RealVolume, v, frame );
  m_Volume = volume;

  if( frame==0 )
  { //  変更時間０ならすぐに変更しておく
    m_Sound.SetVolume( v );
  }
}


//! フェードアウトします
/*!
    アウトしきると、バッファがとまります
 */
void GameSound::FadeOut( double time )
{
  SetVolume( 0, time );
  m_IsFadeOuting = true;
}

void GameSound::SetPosition( double time )
{
  m_Sound.SetPosition( time );
}


//! 再生中か？
/*!
    @return 再生中なら true
 */
bool GameSound::IsPlay() const
{
  return m_Sound.IsPlay();
}


//! 再生位置の取得
/*!
    @return 再生位置（1.0f=１秒）
 */
double GameSound::GetPosition() const
{
  return m_Sound.GetPosition();
}

//! 現在のボリュームの取得
/*!
    @return ボリューム
 */
double GameSound::GetVolume() const
{
  return m_Volume;
}

//! 再生回数の設定
/*!
    @param 再生する回数, LOOPPLAY でずっとループ再生
 */
void GameSound::SetPlayCount( int count )
{
  MAID_ASSERT( count<LOOPPLAY, "負数は LOOPPLAY 以外指定できません" );
  m_PlayCount = count;

}

//! 残っている再生回数の取得
/*!
    @return 再生回数。 現在最後の音がなってるときは０になります。
 */
int  GameSound::GetPlayCount() const
{
  return m_PlayCount;
}

void GameSound::_Play()
{
  if( m_PlayCount==0 ) { return ; }
  if( m_PlayCount==LOOPPLAY || 1<m_PlayCount )
  {
    m_Sound.SetLoopState(true);
  }else
  {
    m_Sound.SetLoopState(false);
  }

  SetVolume(m_Volume,0);
  m_PrevPosition = GetPosition();

  if( m_PlayCount!=LOOPPLAY )
  {
    m_PlayCount -= 1;
  }
  m_IsPlaying = true;
  m_Sound.Play();
}


//! フレーム毎の更新
/*!
    毎フレーム呼ぶ必要がありますが、GameSoundManager が呼ぶので使用者は気にしなくていいです。
 */
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
        return ;
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

  if( m_PlayCount!=LOOPPLAY && m_PlayCount!=0 )
  {
    //  ループ回数をチェックする
    const double pos = m_Sound.GetPosition();

    if( pos < m_PrevPosition )
    { //  前回の位置より今回の位置が手前にある＝＝ループした。

      --m_PlayCount;
      if( m_PlayCount==0 ) { m_Sound.SetLoopState(false); }
      else { m_Sound.SetLoopState(true); }
    }

    m_PrevPosition = pos;
  }
}

void GameSound::OnGlobalVolumeChange()
{
  SetVolume( m_Volume, 0.1 );
}

double GameSound::GetScaleVolume()const
{
  GameSoundManager::TYPE t;

  switch( m_Type )
  {
  case TYPE_GLOBAL: { t = GameSoundManager::TYPE_GLOBAL; }break;
  case TYPE_SE:     { t = GameSoundManager::TYPE_SE; }break;
  case TYPE_BGM:    { t = GameSoundManager::TYPE_BGM; }break;
  case TYPE_VOICE:  { t = GameSoundManager::TYPE_VOICE; }break;
  }

  const double scale = GlobalPointer<GameSoundManager>::Get()->CalcScale( t );

  return scale;
}

bool GameSound::IsEmpty() const
{
  return m_Sound.IsEmpty();
}

String GameSound::GetFileName() const
{
  return m_Sound.GetFileName();
}

}

